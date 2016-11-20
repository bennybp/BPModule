import pulsar as psr
import math

def run_test():
    tester=psr.PyTester("Testing the Space class")
    
    InfiniteVaccuum=psr.Space()
    Infinite=[psr.Space.infinity for i in range(0,3)]
    Sides=[2.0,2.0,2.0]
    ra=math.pi/2
    Cubic=[ra for i in range(0,3)]
    Hexagonal=[ra,ra,120/36*math.pi]
    tester.test_equal("Space is infinite",Infinite,InfiniteVaccuum.lattice_sides)
    tester.test_equal("Space is cubic",Cubic,InfiniteVaccuum.lattice_angles);
    tester.test_function("Space is non-periodic",True,False,InfiniteVaccuum.is_periodic)
        
    HexCell=psr.Space(Hexagonal,Sides)
    tester.test_equal("Hexagonal cell has right sides",Sides,HexCell.lattice_sides)
    tester.test_equal("Hexagonal cell has right angles",Hexagonal,HexCell.lattice_angles)
    tester.test_function("Hexagonal cell is periodic",True,True,HexCell.is_periodic)

    tester.test_function("Inequality operator works",True,True,HexCell.__ne__,InfiniteVaccuum)

    HexCell2=psr.Space(HexCell)
    tester.test_function("Copy constructor works",True,True,HexCell.__eq__,HexCell2)

    tester.print_results()
    return tester.nfailed()
