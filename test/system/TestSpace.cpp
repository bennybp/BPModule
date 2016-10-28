#include "TestCXX.hpp"
#include <pulsar/system/Space.hpp>
#include <pulsar/constants.h>

using namespace pulsar;

std::array<double,3> Infinite={Space::infinity,Space::infinity,Space::infinity},
                     Cubic={PI/2,PI/2,PI/2},
                     Sides={2.0,2.0,2.0},
                     Hexagonal={PI/2,PI/2,120/36*PI};

TEST_CLASS(TestSpace){
    Tester tester("Testing the Space class");
    
    Space InfiniteVaccuum;
    tester.test("Space is infinite",Infinite,InfiniteVaccuum.lattice_sides);
    tester.test("Space is cubic",Cubic,InfiniteVaccuum.lattice_angles);
    tester.test("Space is non-periodic",false,InfiniteVaccuum.is_periodic());
   
    
    Space HexCell(Hexagonal,Sides);
    tester.test("Hexagonal cell has right sides",Sides,HexCell.lattice_sides);
    tester.test("Hexagonal cell has right angles",Hexagonal,HexCell.lattice_angles);
    tester.test("Hexagonal cell is periodic",true,HexCell.is_periodic());
   
    tester.test("Inequality operator works",true,HexCell!=InfiniteVaccuum);
    
    Space HexCell2(HexCell);
    tester.test("Copy constructor works",HexCell,HexCell2);
    Space HexCell3(std::move(HexCell));
    tester.test("Move constructor works",HexCell2,HexCell3);
    HexCell=std::move(HexCell3);
    tester.test("Move assignment works",HexCell,HexCell2);
    HexCell3=HexCell2;
    tester.test("Assignment works",HexCell,HexCell3);
    
    tester.print_results();
}
