#@+leo-ver=4-thin
#@+node:gcross.20090221223144.2:@thin SConstruct
#@@language Python

env = Environment(
            CCFLAGS="-O3"
        )
import os
for varname in ["CPATH","CPP_INCLUDE_PATH"]:
    if varname in os.environ:
        env.AppendENVPath(varname,os.environ[varname])
env.PrependENVPath("CPATH","src")
env.PrependENVPath("CPATH","include")

#@+at
# Check to make sure that Boost is present.
#@-at
#@@c
conf = Configure(env)
if not conf.CheckCXXHeader('boost/dynamic_bitset.hpp'):
   print 'You need to download and install the Boost C++ library to use this code.'
   Exit(1) 
if not conf.CheckCXXHeader('blitz/array.h'):
   print 'You need to download and install the Blitz++ library to use this code.'
   Exit(1) 

#@+at
# Get our configuration options:
#@-at
#@@c
opts = Variables('codequery.conf') # Change wm to the name of your app
opts.Add(PathVariable('PREFIX', 'Directory to install under', '/usr/local', PathVariable.PathIsDir))
opts.Update(env)
opts.Save('codequery.conf', env) # Save, so user doesn't have to 
                          # specify PREFIX every time
Help(opts.GenerateHelpText(env))

#@+at
# Build the program:
#@-at
#@@c
source_file_names = ['codelib.cc'] + ['codequery%s.cc' % suffix for suffix in ['','-sparse','-dense-1d']]
codequery = env.Program('bin/codequery',['src/%s' % filename for filename in source_file_names])
Default(codequery)

p1 = env.Program('testing/randomized-tests',['src/codelib.cc','testing/randomized-tests.cc'])
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
Export('env idir_prefix idir_lib idir_bin idir_inc idir_data')

env.Install(idir_bin, codequery)
env.Install(idir_inc, ['src/codelib.hpp'])
env.Alias('install', idir_prefix)
#@-node:gcross.20090221223144.2:@thin SConstruct
#@-leo
