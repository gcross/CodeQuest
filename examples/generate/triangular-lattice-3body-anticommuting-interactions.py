#!/usr/bin/python
#@+leo-ver=4-thin
#@+node:gmc.20080912133652.2:@thin triangular-lattice-3body-anticommuting-interactions.py
#@@first
from __future__ import with_statement
from numpy import array

import sys

if len(sys.argv) != 2:
    print """\
Usage:

    %s size

Generates a hexagonal lattice with width 2*<size>.
""" % sys.argv[0]
    sys.exit()

size = int(sys.argv[1])

bonds = []

lines = []

directions = [
    ("Y",array([2,0]),array([1,1])),
    ("Z",array([-2,0]),array([-1,1])),
    ("X",array([-1,1]),array([-2,0])),
]

center = array([0,0])

def do_row(starting_x, y, number_of_triangles):
    if starting_x %2 == 0:
        op_number = 0
    else:
        op_number = 1
    #ops = ["X","Y","Z"]
    ops = ["X","Z"]
    for i in xrange(number_of_triangles):
        x = starting_x + i*2
        op = ops[op_number]
        print "(%i,%i) %s (%i,%i) %s (%i,%i) %s" % (x,y,op,x+2,y,op,x+1,y+1,op)
        op_number = (op_number+1)%len(ops)

for row_number in xrange(size+1):
    do_row(row_number,-row_number,size*2-row_number)
    if row_number != size and row_number != 0:
        do_row(row_number,+row_number,size*2-row_number)
#@-node:gmc.20080912133652.2:@thin triangular-lattice-3body-anticommuting-interactions.py
#@-leo
