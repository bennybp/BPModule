/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "pulsar/math/Checking.hpp"
#include "pulsar/math/MinMax.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/system/symmetry/CrystalSystem.hpp"

namespace pulsar{
namespace system{
using std::array;
typedef array<double,3> Vector_t;

CrystalSystem AssignCrystalSystem(const Vector_t& As,
                                  const Vector_t& Ss,double Tol){
    double SideTol=math::max(Ss[0],Ss[1],Ss[2])*Tol;
    double AngleTol=math::max(As[0],As[1],As[2])*Tol;
    int UniqueSides=3,UniqueAngles=3;
    for(size_t i=0;i<3;++i){
        for(size_t j=i+1;j<3;++j){
            if(math::are_equal(Ss[i],Ss[j],SideTol))--UniqueSides;
            if(math::are_equal(As[i],As[j],AngleTol))--UniqueAngles;
        }
    }
    if(UniqueAngles==3&&UniqueSides==3)return Triclinic(As,Ss);
    int N90=0,N120=0;
    for(double Angle:As){
        if(math::are_equal(90.0,Angle,90.0*Tol))++N90;
        else if(math::are_equal(120.0,Angle,120.0*Tol))++N120;
    }
    if(UniqueAngles==0){
        if(N90==3){
            if(UniqueSides==3)return Orthorhombic(As,Ss);
            else if(UniqueSides==2)return Tetragonal(As,Ss);
            else if(UniqueSides==0)return Cubic(As,Ss);
        }
        else if(N90==0)return Rhombohedral(As,Ss);
    }
    if(UniqueAngles==2&&N90==2){
        if(UniqueSides==2&&N120==1)return Hexagonal(As,Ss);
        else return Monoclinic(As,Ss);
    }    
    throw GeneralException("Your angle/sides do not correspond to any of the"
                           "7 possible crystal systems to within the"
                           "specified tolerance.",
                           "Alpha",As[0],"Beta",As[1],"Gamma",As[2],
                           "a",Ss[0],"b",Ss[1],"c",Ss[2],
                           "Percent tolerance",100*Tol);
}


}}//End namespaces
