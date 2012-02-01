#!/usr/bin/python

from read_grid_parameters import width, height

for x in xrange(0,width,2):
    for y in xrange(0,height,2):
        if x % 2 == 0:
            print(("(%i,%i) X "*3) % (x  ,y  ,x+1,y  ,x  ,y+1))
            print(("(%i,%i) Z "*3) % (x+1,y+1,x+1,y  ,x  ,y+1))
        else:
            print(("(%i,%i) X "*3) % (x  ,y  ,x  ,y+1,x+1,y+1))
            print(("(%i,%i) Z "*3) % (x  ,y  ,x+1,y  ,x+1,y+1))
