from TestFxns import *
from math import sqrt

tester = Tester("Testing Universe and MathSet")

#Some coordinates to fill our points with
C1=[1.0,2.0,3.0];C2=[1.0,2.0,3.0]

P1=psr.math.Point(C1);P2=psr.math.Point(C2[0],C2[1],C2[2])

#Testing Constructors
tester.test_value("Constructors and comparison",P1,P2)
C1[0]=10.0;C2[0]=15.0
tester.test_value("Array constructors don't alias",P1,P2)

#Setting Coords
tester.test("Set coords way 1",True,None,P1.set_coords,C2)
C2[1]=18.0;C2[2]=10.0
tester.test("Set coords way 2",True,None,P1.set_coord,1,18.0)
tester.test("Set coords way 3",True,None,P2.set_coords,C2[0],C2[1],C2[2])
tester.test("Set coords way 4",True,None,P2.__setitem__,2,3.0)
tester.test_value("Set coords sanity check",P1,P2)
tester.test("Set index error fails",False,None,P2.set_coord,3,100.0)
tester.test("Set index error operator fails",False,None,P2.__setitem__,3,100.0)

#Retrieving Coords
C2[2]=3.0
tester.test("Get list of coordinates 1",True,C2,P1.get_coords)
C3=P2.get_coords()
C3[0]=77.0
tester.test("Get coords doesn't alias",True,C2[0],P2.get_coord,0)
tester.test("Get coords operator",True,C2[0],P2.__getitem__,0)
tester.test("Get index error fails",False,None,P2.get_coord,3)
tester.test("Get index error fails operator",False,None,P2.__getitem__,3)

#Distance 
tester.test("Distance from literally same point",True,0.0,P1.distance,P1)
P3=psr.math.Point(3.0,4.0,5.0)
dist=sqrt((15.0-3.0)**2+(18.0-4.0)**2+(5.0-3.0)**2)
tester.test("Distance",True,dist,P1.distance,P3)
tester.test_value("Distance is const",P1,P2)

#Magnitude
tester.test("Magnitude",True,sqrt(15**2+18**2+9),P1.magnitude)
tester.test_value("Magnitdue is const",P1,P2)

#Printing
out=psr.output.get_global_output()
tester.test("print function",True,None,P1.print,out)
tester.test("print as string",True,"15 18 3",P1.__str__)
tester.test("print as string",True,"15 18 3",P2.__str__)

#Hashing
corr_hash=[95, 113, 7, 186, 56, 39, 103, 32, 207, 218, 13, 83, 31, 241, 31, 29]
tester.test("Hash function",True,corr_hash,P1.my_hash)
tester.test("Hash function",True,corr_hash,P2.my_hash)

#Operators
tester.test("Inequality operator",True,True,P1.__ne__,P3)
P4=psr.math.Point(18,22,8)
tester.test("Add",True,P4,P1.__add__,P3)
tester.test("+=",True,P4,P1.__iadd__,P3)
tester.test("Subtract",True,P2,P1.__sub__,P3)
tester.test("-=",True,P2,P1.__isub__,P3)
P4.set_coords(15**2,18**2,9)
tester.test("Multiply",True,P4,P1.__mul__,P1)
tester.test("*=",True,P4,P1.__imul__,P1)

tester.print_results()

psr.finalize()
