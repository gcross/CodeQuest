#!/usr/bin/python

from read_grid_parameters import width, height

for x in xrange(1,width+1):
    for y in xrange(1,height+1):
        if x != width:
            print("(%i,%i) X (%i,%i) X" % (x,y,x+1,y))
        if y != height:
            print("(%i,%i) Z (%i,%i) Z" % (x,y,x,y+1))

