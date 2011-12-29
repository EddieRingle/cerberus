env = Environment() # initialize the environment

library_path = "vendor/install/lib" # path of contrib libraries
include_path = "vendor/install/include" # path of contrib includes

env.Append(CFLAGS=['-std=gnu89', '-fno-strict-aliasing', '-Wall',
'-pedantic', '-Werror=declaration-after-statement', '-Wno-long-long',
'-ggdb', '-O0'])

env.Append(CPPPATH = include_path)
env.Append(CPPPATH = ['vendor/opengl'])
env.Append(LIB_PATH = "bin")

env.Library('bin/cerberus', Glob('source/*.c'))

env.Program('bin/crbtest', Glob('test/*.c'),
                            LIBS = ['cerberus', 'SDL', 'SDL_image', 
'GL', 'GLEW', 'luajit-5.1', 'm', 'dl', 'pthread'],
                            LIBPATH = ['bin', 'vendor/install/lib'])
