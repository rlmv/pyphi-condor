from quacker import quack

cdef extern from "output.h":
    cpdef void out()

cdef extern from "MasterMain_test.H":
    cdef int start();

cpdef public void call_quack():
    quack()

cpdef start_mw():
    start()

cdef public void c_quack():
    quack()
