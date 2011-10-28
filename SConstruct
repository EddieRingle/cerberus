env = Environment() # initialize the environment

library_path = "contrib/install/lib" # path of contrib libraries
include_path = "contrib/install/include" # path of contrib includes

env.Append(CFLAGS=['-std=gnu89', '-fno-strict-aliasing', '-Wall',
'-pedantic', '-Werror=declaration-after-statement'])

env.Append(CPPPATH = include_path, LIBPATH = library_path,
        LIBS = ['crisscross'])

env.Library('bin/cerberus', Glob('source/*.c'))
