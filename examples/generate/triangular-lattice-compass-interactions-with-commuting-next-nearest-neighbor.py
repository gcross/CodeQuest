#!/usr/bin/python
#@+leo-ver=4-thin
#@+node:gmc.20080912133652.4:@thin triangular-lattice-compass-interactions-with-commuting-next-nearest-neighbor.py
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

for op, forward, left in directions:
    def add_line(coord_1,coord_2):
        print "%s %s %s %s" % (tuple(coord_1),op,tuple(coord_2),op)
    for i in xrange(size):
        add_line(center+left*i,center+left*(i+1))
        add_line(center-left*i,center-left*(i+1))
        if i != (size-1):
            add_line(center+left*i,center+left*(i+2))
            add_line(center-left*i,center-left*(i+2))
    add_line(center-left,center+left)


    for j in xrange(1,size+1):
        position = center+forward*j
        for i in xrange(size):
            add_line(position-left*i,position-left*(i+1))
            if i != (size-1):
                add_line(position-left*i,position-left*(i+2))
        for i in xrange(size-j):
            add_line(position+left*i,position+left*(i+1))
            if i != (size-j-1):
                add_line(position+left*i,position+left*(i+2))
        if j != size:
            add_line(position-left,position+left)
    for j in xrange(1,size+1):
        position = center-forward*j
        for i in xrange(size):
            add_line(position+left*i,position+left*(i+1))
            if i != (size-1):
                add_line(position+left*i,position+left*(i+2))
        for i in xrange(size-j):
            add_line(position-left*i,position-left*(i+1))
            if i != (size-j-1):
                add_line(position-left*i,position-left*(i+2))
        if j != size:
            add_line(position+left,position-left)
#@-node:gmc.20080912133652.4:@thin triangular-lattice-compass-interactions-with-commuting-next-nearest-neighbor.py
#@-leo
