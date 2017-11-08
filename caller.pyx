import pickle


cdef extern from "master_main.h":
    cdef int start(char* pickle_str, int size);


cdef extern from "MW.h":
    cdef void MWprintf ( int level, char *fmt, ... )
    cdef enum MWReturn:
        OK
        QUIT
        ABORT


cpdef start_mw(obj):
    pickle_str = pickle.dumps(obj)
    size = len(pickle_str)
    start(pickle_str, size)


cdef extern from "Python.h":
    struct PyObject
    object PyBytes_FromStringAndSize(char *s, Py_ssize_t len)
    char* PyBytes_AsString(PyObject *o)


cdef public unpack_pickle(char* pickle_str, int pickle_size):
    unpickle = PyBytes_FromStringAndSize(pickle_str, pickle_size)
    obj = pickle.loads(unpickle)

    mw_print(30, '-' * 50 + '\n')
    mw_print(30, "Unpickled %s\n" % obj)

    return obj


cdef public MWReturn use_pickle(python_worker) except *:
    mw_print(30, "Type %s\n" % type(python_worker))
    mw_print(30, "Executing %s\n" % python_worker)

    return OK;


# TODO: there is probably a better way to deal handle encoding
def mw_print(level, py_string):
    '''Print a string through master-worker.'''
    py_string = py_string.encode('utf8')
    # cdef char* c_string = <bytes> py_string
    MWprintf(level, py_string)
