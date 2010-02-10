#@+leo-ver=4-thin
#@+node:gmc.20080914232002.4:@thin quantum_turtle.py
from math import *
import sys

from pylogo.common import *

from bisect import bisect

def get_closest(v,seen_values):
    position = bisect(seen_values,v)
    if position > 0:
        if abs(seen_values[position-1]-v)<1:
            return position-1
    if position < len(seen_values):
        if abs(seen_values[position]-v)<1:
            return position

def seen_id(v,seen_values):
    position = bisect(seen_values,v)
    if position > 0:
        if abs(seen_values[position-1]-v)<1:
            return
    if position < len(seen_values):
        if abs(seen_values[position]-v)<1:
            return
    seen_values.insert(position,v)

class Turtle:

    def forward_to_other_turtle(method):
        def f(self,*args,**keywords):
            method(self,*args,**keywords)
            if self._other_turtle:
                return getattr(self._other_turtle,method.__name__)(*args,**keywords)
        return f

    def __init__(self,_other_turtle=None):
        self._other_turtle = _other_turtle
        self.setheading(0)
        self.x = self.y = 0
        self.seen_x_values = []
        self.seen_y_values = []
        self._pen_is_down = True
        self.color = "black"
        self.operators = {}


    def __repr__(self):
        return '<%s %i>' % (self.__class__.__name__,
                            self._count)



    #@    @+others
    #@+node:gmc.20080914232002.5:Methods that affect operator creation
    @logofunc(aliases=['fd'],arity=1)
    @forward_to_other_turtle
    def forward(self, distance):
        if self._pen_is_down:
            old_x = self.x
            old_y = self.y
        self.x += float(distance) * cos(self.angle)
        self.y += float(distance) * sin(self.angle)
        if self._pen_is_down:
            self.add_operator(old_x,old_y,self.x,self.y)


    @logofunc(aliases=['back', 'bk'],arity=1)
    @forward_to_other_turtle
    def backward(self, distance):
        if self._pen_is_down:
            old_x = self.x
            old_y = self.y
        self.x -= float(distance) * cos(self.angle)
        self.y -= float(distance) * sin(self.angle)
        if self._pen_is_down:
            self.add_operator(old_x,old_y,self.x,self.y)

    @logofunc(aliases=['lt'],arity=1)
    def left(self, rotation_in_degrees):
        self.setheading(self.angle_in_degrees + rotation_in_degrees)

    @logofunc(aliases=['rt'],arity=1)
    def right(self, rotation_in_degrees):
        self.setheading(self.angle_in_degrees - rotation_in_degrees)

    @logofunc(aliases=['pu'])
    @forward_to_other_turtle
    def penup(self):
        self._pen_is_down = False

    @logofunc(aliases=['pd'])
    @forward_to_other_turtle
    def pendown(self):
        self._pen_is_down = True

    @logofunc(arity=1)
    @forward_to_other_turtle
    def setx(self, x):
        self.x = x

    @logofunc(arity=1)
    @forward_to_other_turtle
    def sety(self, y):
        self.y = y

    @logofunc()
    @forward_to_other_turtle
    def home(self):
        self.setheading(0)
        self.x = self.y = 0.0

    @logofunc(aliases=['pc', 'color'],
              arity=1)
    @forward_to_other_turtle
    def pencolor(self, color):
        self.color = color.lower()


    @logofunc(arity=1)
    @forward_to_other_turtle
    def setheading(self, angle_in_degrees):
        self.angle_in_degrees = angle_in_degrees % 360
        self.angle = float(angle_in_degrees) * pi / 180.0

    #@+at
    # @logofunc(arity=2)
    # @forward_to_other_turtle
    # def setxy(self, x, y):
    #     self.x = x
    #     self.y = y
    #@-at
    #@@c
    #@-node:gmc.20080914232002.5:Methods that affect operator creation
    #@+node:gmc.20080914232002.6:Purely display-oriented methods
    @logofunc(aliases=['pc3', 'color3'],
              arity=3)
    @forward_to_other_turtle
    def pencolor3(self, *args):
        pass

    @logofunc(aliases=['ht'])
    @forward_to_other_turtle
    def hideturtle(self):
        pass

    @logofunc(aliases=['st'])
    @forward_to_other_turtle
    def showturtle(self):
        pass

    @logofunc(aliases=['turtleprint', 'turtlepr'], arity=1)
    @forward_to_other_turtle
    def turtlewrite(self, text, move=False):
        pass

    @logofunc()
    @forward_to_other_turtle
    def startfill(self):
        pass

    @logofunc()
    @forward_to_other_turtle
    def endfill(self):
        pass

    @logofunc(arity=1)
    @forward_to_other_turtle
    def speed(self, x):
        pass

    @logofunc(arity=1)
    @forward_to_other_turtle
    def delay(self, x):
        pass

    @logofunc(aliases=['pw'],arity=1)
    @forward_to_other_turtle
    def penwidth(self, v):
        pass


    @logofunc(aliases=['cs', 'clearscreen'])
    @forward_to_other_turtle
    def clear(self):
        pass
    #@nonl
    #@-node:gmc.20080914232002.6:Purely display-oriented methods
    #@+node:gmc.20080914232002.7:Informational Methods
    #@+at
    # 
    # @logofunc()
    # def posx(self):
    #     return self.x
    # 
    # @logofunc()
    # def posy(self):
    #     return self.y
    # 
    # @logofunc()
    # def heading(self):
    #     return self.angle_in_degrees
    # 
    #@-at
    #@@c

    #@-node:gmc.20080914232002.7:Informational Methods
    #@+node:gmc.20080914232002.9:Operator management
    def find_nearby_position(self,operator_position):
        for other_operator_position in self.operators.iterkeys():
            if all(abs(operator_position[i]-other_operator_position[i]) < 5 for i in xrange(4)):
                return other_operator_position
            if all(abs(operator_position[(i+2)%4]-other_operator_position[i]) < 5 for i in xrange(4)):
                return other_operator_position
        return operator_position


    def add_operator(self,*operator_position):
        if self.color in ["red","blue","green"]:
            self.seen_x(operator_position[0])
            self.seen_y(operator_position[1])
            self.seen_x(operator_position[2])
            self.seen_y(operator_position[3])
        if self.color == "white":
            nearby_position = self.find_nearby_position(operator_position)
            if nearby_position in self.operators:
                del self.operators[nearby_position]
        elif self.color == "red":
            self.operators[self.find_nearby_position(operator_position)] = "X"
        elif self.color == "green":
            self.operators[self.find_nearby_position(operator_position)] = "Y"
        elif self.color == "blue":
            self.operators[self.find_nearby_position(operator_position)] = "Z"


    def seen_x(self,x):
        seen_id(x,self.seen_x_values)

    def seen_y(self,y):
        seen_id(y,self.seen_y_values)
    #@-node:gmc.20080914232002.9:Operator management
    #@+node:gmc.20080914232002.8:Miscellaneous
    @logofunc(aware=True)
    def clone(self, interp):
        new = self.__class__()

    @logofunc()
    def turtle(self):
        return self

    @logofunc()
    def info(self):
        print "(%s,%s) @ %s " % (self.x,self.y,self.angle_in_degrees)
        print "x positions: ", self.seen_x_values
        print "y positions: ", self.seen_y_values


    @logofunc(arity=1)
    def postscript(self,filename):
        if self._other_turtle:
            from pylogo.ide import add_command, get_canvas
            canvas = get_canvas()
            width = canvas.winfo_width()
            height = canvas.winfo_height()
            if width <= 1:
                width = canvas['width']
            if height <= 1:
                height = canvas['height']
            x_origin, y_origin = 0, 0
            def printit():
                f = open(filename,"w")
                print >> f, canvas.postscript(
                    width=self.seen_x_values[-1]-self.seen_x_values[0]+35,
                    height=self.seen_y_values[-1]-self.seen_y_values[0]+35,
                    x=x_origin+self.seen_x_values[0]-25,
                    y=y_origin-self.seen_y_values[-1]-10
                )
                f.flush()
                f.close()
            add_command(printit)

    @logofunc()
    def finished(self):
        if self._other_turtle:
            from pylogo.ide import add_command, get_canvas
            canvas = get_canvas()
            x_origin, y_origin = 0, 0
            for x in self.seen_x_values:
                for y in self.seen_y_values:
                    add_command(canvas.create_oval,(x_origin+x-2,y_origin-y-2,x_origin+x+2,y_origin-y+2))
            for i,x in enumerate(self.seen_x_values):
                add_command(canvas.create_text(x_origin+x,y_origin-self.seen_y_values[0]+15,text=str(i)))
            for i,y in enumerate(self.seen_y_values):
                add_command(canvas.create_text(x_origin+self.seen_x_values[0]-15,y_origin-y,text=str(i)))
            add_command(canvas.update)
        for (old_x,old_y,new_x,new_y), op in self.operators.iteritems():
            old_x_grid = get_closest(old_x,self.seen_x_values)
            new_x_grid = get_closest(new_x,self.seen_x_values)
            old_y_grid = get_closest(old_y,self.seen_y_values)
            new_y_grid = get_closest(new_y,self.seen_y_values)
            sys.__stdout__.write("(%i,%i) %s (%i,%i) %s\n" % (old_x_grid,old_y_grid,op,new_x_grid,new_y_grid,op))
        sys.__stdout__.close()

    @logofunc()
    def guibye(self):
        from pylogo.builtins import bye
        if self._other_turtle:
            from pylogo.ide import add_command
            add_command(bye)
        else:
            bye()
    #@-node:gmc.20080914232002.8:Miscellaneous
    #@-others

    del forward_to_other_turtle

@logofunc(aware=True)
def createturtle(interp,gui):
    if gui:
        import pylogo.logo_turtle
        t = Turtle(pylogo.logo_turtle.Turtle())
    else:
        t = Turtle()
    interp.push_actor(t)
#@-node:gmc.20080914232002.4:@thin quantum_turtle.py
#@-leo
