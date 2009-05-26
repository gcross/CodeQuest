#!/usr/bin/python
#@+leo-ver=4-thin
#@+node:gmc.20080911170647.6:@thin compass-interactions-with-3body-XXX-ZZZ.py
#@@first

from read_grid_parameters import width, height

for x in xrange(0,width,2):
    for y in xrange(0,height,2):
        if x % 2 == 0:
            print(("(%i,%i) X "*3) % (x  ,y  ,x+1,y  ,x  ,y+1))
            print(("(%i,%i) Z "*3) % (x+1,y+1,x+1,y  ,x  ,y+1))
        else:
            print(("(%i,%i) X "*3) % (x  ,y  ,x  ,y+1,x+1,y+1))
            print(("(%i,%i) Z "*3) % (x  ,y  ,x+1,y  ,x+1,y+1))

for x in xrange(1,width,2):
    for y in xrange(height+1):
        print(("(%i,%i) X "*2) % (x  ,y  ,x+1,y  ))

for x in xrange(width+1):
    for y in xrange(1,height,2):
        print(("(%i,%i) Z "*2) % (x  ,y  ,x  ,y+1))

#@-node:gmc.20080911170647.6:@thin compass-interactions-with-3body-XXX-ZZZ.py
#@-leo
