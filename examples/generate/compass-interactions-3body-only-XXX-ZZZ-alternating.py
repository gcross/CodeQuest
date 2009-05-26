#!/usr/bin/python
#@+leo-ver=4-thin
#@+node:gmc.20080912012149.12:@thin compass-interactions-3body-only-XXX-ZZZ-alternating.py
#@@first

from read_grid_parameters import width, height

for x in xrange(0,width,1):
    for y in xrange(0,height,1):
        if (x+y) % 2 == 0:
            print(("(%i,%i) X "*3) % (x  ,y  ,x+1,y  ,x  ,y+1))
        else:
            print(("(%i,%i) Z "*3) % (x  ,y  ,x+1,y  ,x  ,y+1))
#@-node:gmc.20080912012149.12:@thin compass-interactions-3body-only-XXX-ZZZ-alternating.py
#@-leo
