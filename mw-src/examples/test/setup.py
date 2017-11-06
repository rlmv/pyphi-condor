from distutils.core import setup, Extension


module1 = Extension('demo',
                    # define_macros = [('MAJOR_VERSION', '1'),
                    #                  ('MINOR_VERSION', '0')],
                    # include_dirs = ['/usr/local/include'],
                    # libraries = ['tcl83'],
                    # library_dirs = ['/usr/local/lib'],
                    sources=['master.pyx'],
                    language='c++')

setup(ext_modules = [module1])
