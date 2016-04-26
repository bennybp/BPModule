/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <iostream>
#include "pulsar/system/symmetry/SymmetryElements.hpp"
#include "pulsar/math/Geometry.hpp"

    using std::string;
    using std::to_string;
    typedef std::array<double,3> Vector_t;
    typedef std::array<double,9> Matrix_t;

namespace pulsar{
namespace system{
    

    
bool SymmetryElement::operator==(const SymmetryElement& Other)const{
    const double Tol=1.0e-2;
    bool MatSame=true;
    for(size_t i=0;i<9&&MatSame;++i)
        MatSame=(fabs(Elem[i]-Other.Elem[i])<Tol);
    return MatSame && SSymbol==Other.SSymbol && HMSymbol==Other.HMSymbol;
}
    
std::ostream& SymmetryElement::Print(std::ostream& os)const{
        os<<SSymbol;
        for(double i: Elem)os<<" "<<i;
        return os;
}


    
const SymmetryElement Identity({1.0,0.0,0.0,
                                0.0,1.0,0.0,
                                0.0,0.0,1.0},"E","1");    


const SymmetryElement CoI({-1.0,0.0,0.0,
                            0.0,-1.0,0.0,
                            0.0,0.0,-1.0},"i","[1]");

inline double ToDegrees(size_t n,size_t m){
    return 360.0/(static_cast<double>(n)/static_cast<double>(m));
}                            

inline std::string SSym(size_t n,size_t m){
    return to_string(n)+(m>1?"^"+to_string(m):"");
}

Rotation::Rotation(const Vector_t& Axis,size_t n,size_t m):
    SymmetryElement(math::Rotation(Axis,ToDegrees(n,m)),"C_"+SSym(n,m),
                    to_string(n)){}                            

MirrorPlane::MirrorPlane(const Vector_t& Norm):
    SymmetryElement(math::Reflection(Norm),"s","m"){}

ImproperRotation::ImproperRotation(const Vector_t& Axis,size_t n,size_t m):
    SymmetryElement(math::RotoReflection(Axis,ToDegrees(n,m)),"S_"+SSym(n,m),
                    "["+to_string(n%2==1?n*2:(n%4==0?n:n/2))+"]"){}
    
}}//End namespaces


