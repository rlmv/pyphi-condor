from quacker import quack

import pickle

cdef extern from "output.h":
    cpdef void out()

cdef extern from "MasterMain_test.H":
    cdef int start(char* pickle_str, int size);

cpdef public void call_quack():
    quack()

cpdef start_mw(obj):
    pickle_str = pickle.dumps(obj)
    size = len(pickle_str)
    start(pickle_str, size)

cdef public void c_quack():
    quack()

cdef extern from "Python.h":
    object PyBytes_FromStringAndSize(char *s, Py_ssize_t len)


cdef public void check_pickle(char* pickle_str, int pickle_size) except *:
    unpickle = PyBytes_FromStringAndSize(pickle_str, pickle_size)
    print("-" * 50)

    obj = pickle.loads(unpickle)
    print(obj)
