import pickle

from cpython cimport Py_INCREF

from cpython.mem cimport PyMem_Malloc, PyMem_Free
from cpython.object cimport PyObject
from cpython.bytes cimport PyBytes_FromStringAndSize
from cpython.unicode cimport PyUnicode_AsUTF8String


cdef extern from "Python.h":
    ctypedef PyObject PyListObject


cdef extern from "driver.h":
    cdef cppclass Driver:
        Driver(char*, int, PyListObject*)
        void go( int argc, char *argv[] )

cdef extern from "worker.h":
    cdef cppclass Worker:
        Worker()
        void go( int argc, char *argv[] )


cdef extern from "MW.h":
    cdef void MWprintf ( int level, char *fmt, ... )
    cdef void set_MWprintf_level(int level)
    cdef enum MWReturn:
        OK
        QUIT
        ABORT


cpdef start_mw(obj, cuts):
    pickle_str = pickle.dumps(obj)
    size = len(pickle_str)

    cdef Driver *driver = new Driver(pickle_str, size, <PyListObject*> cuts)
    set_MWprintf_level(75)
    mw_print(10, "The master is starting.\n")

    driver.go(0, [])  # No argc/v here


cdef public unpack_pickle(char* pickle_str, int pickle_size):
    unpickle = PyBytes_FromStringAndSize(pickle_str, pickle_size)
    obj = pickle.loads(unpickle)

    mw_print(30, "Unpickled %s\n" % str(obj))

    return obj


# TODO: what needs to happen with reference counting here?
cdef public void pack_pickle(obj, char** pickle_str, int* pickle_size) except *:

    py_pickle_str = pickle.dumps(obj)
    pickle_size[0] = len(py_pickle_str)
#    Py_INCREF(py_pickle_str)
    pickle_str[0] = py_pickle_str


# TODO: error handling
cdef public use_pickle(python_worker, job):
    mw_print(30, "Type %s\n" % type(python_worker))
    mw_print(30, "Executing %s\n" % python_worker)

    print(job)

    mw_print(30, "Result of {} + {} = {}\n".format(
        python_worker.x, job, python_worker.run(job)))

    return python_worker.run(job)


cdef public void print_result(result):
    print('=' * 20)
    print('Result: %s' % result)
    print('=' * 20)


# TODO: there is probably a better way to deal handle encoding
def mw_print(level, py_string):
    '''Print a string through master-worker.'''
    py_string = py_string.encode('utf8')
    # cdef char* c_string = <bytes> py_string
    MWprintf(level, py_string)


def start_worker(py_argc, py_argv):
    cdef int argc = py_argc
    cdef Worker *worker
    cdef char** argv

    set_MWprintf_level(95)
    mw_print(10, "A worker is starting.\n")

    argv = <char**>PyMem_Malloc(argc * sizeof(char*))
    if argv == NULL:
        raise MemoryError()

    try:
        args = [PyUnicode_AsUTF8String(arg) for arg in py_argv]
        for i in range(argc):
            argv[i] = args[i]
            if argv[i] == NULL:
                raise Exception('Issue encoding argv')

            MWprintf(30, "Arg %d %s\n", <int>i, argv[i])

        worker = new Worker()
        worker.go(argc, argv)

    finally:
        PyMem_Free(argv)

    return 0
