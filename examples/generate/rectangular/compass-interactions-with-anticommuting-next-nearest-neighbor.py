#!/usr/bin/python

from read_grid_parameters import width, height

for x in xrange(width):
    for y in xrange(height):
        if x != width-1:
            print("(%i,%i) X (%i,%i) X" % (x,y,(x+1)%width,y))
        if y != height-1:
            print("(%i,%i) Z (%i,%i) Z" % (x,y,x,(y+1)%height))
        if x != width-2:
            print("(%i,%i) Z (%i,%i) Z" % (x,y,(x+2)%width,y))
        if y != height-2:
            print("(%i,%i) X (%i,%i) X" % (x,y,x,(y+2)%height))
