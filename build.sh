cython *.pyx
cc -c *.c -I/System/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7
cc -L/System/Library/Frameworks/Python.framework/Versions/2.7/lib -L/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config -lpython2.7 -ldl *.o -o main
