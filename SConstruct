env = Environment() # initialize the environment

library_path = "contrib/install/lib" # path of contrib libraries
include_path = "contrib/install/include" # path of contrib includes

# Basic CFLAGS for correctness
env.Append(CFLAGS=['-std=c99', '-fno-strict-aliasing'])
# Warning flags
#env.Append(CFLAGS='-Wall -Wextra -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Werror=declaration-after-statement')

env.Append(CPPPATH = include_path, LIBPATH = library_path,
        LIBS = ['crisscross'])

Library('bin/cerberus', Glob('source/*.c'))
