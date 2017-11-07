from quacker import quack

cdef extern from "output.h":
    cpdef void out()

cdef extern from "MasterMain_test.H":
    cdef int start(char* pickle, int size);

cpdef public void call_quack():
    quack()

cpdef start_mw(pickle):
    size = len(pickle)
    start(pickle, size)

cdef public void c_quack():
    quack()


cdef extern from "Python.h":
    object PyBytes_FromStringAndSize(char *s, Py_ssize_t len)

cdef public void check_pickle(char* pickle, int pickle_size):
    unpickle = PyBytes_FromStringAndSize(pickle, pickle_size)
    print("-" * 50)
    print(unpickle)
