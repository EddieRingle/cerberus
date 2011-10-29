env = Environment() # initialize the environment

library_path = "contrib/install/lib" # path of contrib libraries
include_path = "contrib/install/include" # path of contrib includes

env.Append(CFLAGS=['-std=gnu89', '-fno-strict-aliasing', '-Wall',
'-pedantic', '-Werror=declaration-after-statement', '-Wno-long-long',
'-ggdb', '-O0'])

env.Append(CPPPATH = include_path)

env.Library('bin/cerberus', Glob('source/*.c'))

env.Append(LIBPATH = ['bin'], LIBS = ['cerberus'])

env.Program('bin/crbtest', Glob('test/*.c'))
