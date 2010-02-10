#!/usr/bin/env python
#@+leo-ver=4-thin
#@+node:gmc.20080914232002.3:@thin __init__.py
#@@first
#@@language python

import os, sys
sys.stdout = sys.stderr

import optparse

import os.path

try:
    here = __file__
except NameError:
    here = sys.argv[0]

parser = optparse.OptionParser(usage='%prog [OPTIONS]')
parser.add_option(
    '-g', '--gui',
    help="Show the GUI",
    action="store_true",
    dest="gui")

from pylogo import Logo

def idemain(filenames):
    import sys
    from pylogo import ide, interpreter
    from tilings_interpreter import quantum_turtle
    ide.TheApp = ide.IDE(ide.root)
    ide.TheApp.input.component('entry').focus_force()
    comm = ide.LogoCommunicator(ide.TheApp, interpreter.Logo)
    sys.stdout = comm

    interpreter.Logo.import_module(quantum_turtle)
    quantum_turtle.createturtle(interpreter.Logo,True)

    for filename in filenames:
        interpreter.Logo.import_logo(filename)

    comm.start()

    ide.root.mainloop()


def main():
    doit(sys.argv[1:])

def doit(args):
    from tilings_interpreter import arguments
    Logo.import_module(arguments)

    options, args = parser.parse_args(args)

    if "-" in args:
        argposition = args.index("-")+1
        fileposition = argposition-1
    else:
        argposition = len(args)
        fileposition = len(args)

    filenames = [os.path.join(os.path.dirname(here),"tilings_library.logo")] + args[:fileposition]

    arguments.set_args(args[argposition:])

    if options.gui:
        idemain(filenames)
    else:
        from tilings_interpreter import quantum_turtle
        Logo.import_module(quantum_turtle)
        quantum_turtle.createturtle(Logo,False)

        for filename in filenames:
            Logo.import_logo(filename)
        sys.stdout = sys.__stdout__
        Logo.input_loop(sys.stdin, sys.stdout)

if __name__ == "__main__":
    main()
#@-node:gmc.20080914232002.3:@thin __init__.py
#@-leo
