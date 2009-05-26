#!/usr/bin/python
#@+leo-ver=4-thin
#@+node:gmc.20080911170647.2:@thin random-compass-interactions.py
#@@first

from read_grid_parameters import width, height

from random import choice

ops = ["X","Y","Z"]

arr = []

for x in xrange(1,width+1):
    for y in xrange(1,height+1):
        if x != width:
            print("(%i,%i) %%s (%i,%i) %%s" % (x,y,x+1,y) % ((choice(ops),)*2))
        if y != height:
            print("(%i,%i) %%s (%i,%i) %%s" % (x,y,x,y+1) % ((choice(ops),)*2))
        #op = choice(["I","X","Y","Z"])
        #if op != "I":
        #    print("(%i,%i) %s" % (x,y,op))
#@-node:gmc.20080911170647.2:@thin random-compass-interactions.py
#@-leo
