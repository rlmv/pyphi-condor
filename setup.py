from distutils.core import setup
from Cython.Build import cythonize

from distutils.core import setup, Extension
import os
from glob import glob

MW_DIR = os.path.abspath('./mw-src')
INC_DIR = os.path.join(MW_DIR, 'src')
LIB_DIR = os.path.abspath('./mw-build/lib')


module1 = Extension('caller',
                    sources=[
                        'caller.pyx',
#                        'hello.pyx',
                        'output.cpp'] + glob('*.C'),
                    language='c++',
                    define_macros=[('INDEPENDENT', 1)],
                    include_dirs=[
                        INC_DIR,
                        os.path.abspath('.'),
                        os.path.join(INC_DIR, 'RMComm'),
                        os.path.join(INC_DIR, 'RMComm/MWControlTasks'),
                        os.path.join(INC_DIR, 'RMComm/MW-Independent')
                    ],
                    library_dirs=[
                        LIB_DIR
                    ],
                    libraries=[
                        'MW_indp',
                        'MWRMComm_indp',
                        'MWRC_indp',
                        'MWutil_indp',
                        'NWS_indp'],
                    extra_compile_args=['-Wno-c++11-compat-deprecated-writable-strings']
)

setup(
  name='Hello world app',
  ext_modules=cythonize(module1)
)
