from distutils.core import setup
from Cython.Build import cythonize

from distutils.core import setup, Extension
import os
from glob import glob


INC_DIR = os.path.abspath('../mw-build/include')
LIB_DIR = os.path.abspath('../mw-build/lib')


module1 = Extension('caller',
                    sources=[
                        'caller.pyx',
                        'worker_exec.pyx',
                        ] + glob('*.cpp'),
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
                    extra_compile_args=[
                    ],
                    extra_link_args=[
                    ]
)

setup(
  name='Hello world app',
  ext_modules=cythonize(module1)
)
