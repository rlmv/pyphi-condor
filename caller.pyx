from quacker import quack

import pickle

cdef extern from "output.h":
    cpdef void out()

cdef extern from "MasterMain_test.H":
    cdef int start(char* pickle_str, int size)

cdef extern from "MW.h":
    cdef void MWprintf ( int level, char *fmt, ... )
    cdef enum MWReturn:
        OK
        QUIT
        ABORT

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

cdef public unpack_pickle(char* pickle_str, int pickle_size):
    unpickle = PyBytes_FromStringAndSize(pickle_str, pickle_size)
    print("-" * 50)
    obj = pickle.loads(unpickle)
    print("Unpickled", obj)

    return obj


cdef public MWReturn use_pickle(python_worker):
   MWprintf(30, 'Executing {}'.format(python_worker))

   return OK;
