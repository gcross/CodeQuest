#@+leo-ver=4-thin
#@+node:gmc.20080911170647.8:@thin read_grid_parameters.py
__all__ = ["width","height"]

import sys

if len(sys.argv) < 2 or len(sys.argv) > 3:
    print """\
Usage:

    %s width [height]

Generates a compass model of size <width> x <height>.
""" % sys.argv[0]
    sys.exit()

width = int(sys.argv[1])

if len(sys.argv) == 3:
    height = int(sys.argv[2])
else:
    height = width
#@-node:gmc.20080911170647.8:@thin read_grid_parameters.py
#@-leo
