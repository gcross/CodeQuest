#@+leo-ver=4-thin
#@+node:gcross.20090221223144.2:@thin SConstruct
#@@language Python

env = Environment(
            CCFLAGS="-O3 -funroll-loops"
        )
import os
for varname in ["CPATH","CPP_INCLUDE_PATH"]:
    if varname in os.environ:
        env.AppendENVPath(varname,os.environ[varname])
env.PrependENVPath("CPATH","src")

#@+at
# Check to make sure that Boost is present.
#@-at
#@@c
errors = []
conf = Configure(env)
if not conf.CheckCXXHeader('boost/dynamic_bitset.hpp'):
   errors.append('You need to download and install the Boost C++ library to use this code.')
if not conf.CheckCXXHeader('tclap/CmdLine.h'):
   errors.append('You need to download and install the TCLAP library to use this code.')
if errors:
    for error in errors:
        print error
    Exit(1)

#@+at
# Get our configuration options:
#@-at
#@@c
opts = Variables('codequest.conf') # Change wm to the name of your app
opts.Add(PathVariable('PREFIX', 'Directory to install under', '/usr/local', PathVariable.PathIsDir))
opts.Update(env)
opts.Save('codequest.conf', env) # Save, so user doesn't have to 
                          # specify PREFIX every time
Help(opts.GenerateHelpText(env))

#@+at
# Build the program:
#@-at
#@@c
source_file_names = ['codequest%s.cc' % suffix for suffix in ['','-sparse','-dense-1d']]
codequest = env.Program('bin/codequest',['src/%s' % filename for filename in source_file_names])
Default(codequest)

p1 = env.Program('testing/randomized-tests',['testing/randomized-tests.cc'])
env.Alias('test',p1)

#@+at
# Perform installation:
#@-at
#@@c
idir_prefix = '$PREFIX'
idir_lib    = '$PREFIX/lib'
idir_bin    = '$PREFIX/bin'
idir_inc    = '$PREFIX/include'
idir_data   = '$PREFIX/share'
idir_man1   = '$PREFIX/share/man/man1'
Export('env idir_prefix idir_lib idir_bin idir_inc idir_data')

env.Install(idir_bin, codequest)
env.Install(idir_inc, ['src/codequest.hpp'])
env.Install(idir_man1, ['man/codequest.1'])
env.Alias('install', idir_prefix)
#@nonl
#@-node:gcross.20090221223144.2:@thin SConstruct
#@-leo
