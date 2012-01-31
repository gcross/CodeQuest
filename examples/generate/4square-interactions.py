#!/usr/bin/python

from read_grid_parameters import width, height

for x in xrange(width-1):
    for y in xrange(height-1):
        coordinates = (x,y,x+1,y,x,y+1,x+1,y+1)
        print(("(%i,%i) X "*4) % coordinates)
        print(("(%i,%i) Z "*4) % coordinates)
