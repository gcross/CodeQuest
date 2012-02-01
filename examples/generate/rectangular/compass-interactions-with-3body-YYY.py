#!/usr/bin/python

from read_grid_parameters import width, height

for x in xrange(width+1):
    for y in xrange(height+1):
        if x != width:
            print("(%i,%i) X (%i,%i) X" % (x,y,x+1,y))
        if y != height:
            print("(%i,%i) Z (%i,%i) Z" % (x,y,x,y+1))

#        if x != width and y != height:
#            print(("(%i,%i) X (%i,%i) Z (%i,%i) Z") % (x  ,y  ,x+1,y  ,x  ,y+1))
#            print(("(%i,%i) Z (%i,%i) X (%i,%i) X") % (x+1,y+1,x+1,y  ,x  ,y+1))
        if x != width and y != height:
            print(("(%i,%i) Y (%i,%i) Y (%i,%i) Y") % (x  ,y  ,x+1,y  ,x  ,y+1))
            print(("(%i,%i) Y (%i,%i) Y (%i,%i) Y") % (x+1,y+1,x+1,y  ,x  ,y+1))
