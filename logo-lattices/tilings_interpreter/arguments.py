#@+leo-ver=4-thin
#@+node:gmc.20080914232002.10:@thin arguments.py
#@@language python

from pylogo.common import logofunc

__all__ = ["numargs","getiarg","getsarg"]

def set_args(args_):
    global args
    args = args_

@logofunc()
def numargs():
    return len(args)

@logofunc(arity=1)
def getiarg(argindex):
    return int(args[int(argindex)-1])

@logofunc(arity=1)
def getsarg(argindex):
    return args[int(argindex)-1]

#@-node:gmc.20080914232002.10:@thin arguments.py
#@-leo
