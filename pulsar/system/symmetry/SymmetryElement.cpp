/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <iostream>
#include "pulsar/system/symmetry/SymmetryElement.hpp"
#include "pulsar/math/Geometry.hpp"
#include "pulsar/math/MinMax.hpp"

    using std::string;
    using std::to_string;
    typedef std::array<double,3> Vector_t;
    typedef std::array<double,9> Matrix_t;

namespace pulsar{
namespace system{
    

    
bool SymmetryElement::operator==(const SymmetryElement& Other)const{
    double MaxVal=0.0;
    for(size_t i=0;i<9;++i)MaxVal=max_abs(MaxVal,Other.element_matrix[i]);
    double Tol=0.1*MaxVal;
    bool MatSame=true;
    for(size_t i=0;i<9&&MatSame;++i)
        MatSame=(fabs(element_matrix[i]-Other.element_matrix[i])<Tol);
    return MatSame && schoenflies_symbol==Other.schoenflies_symbol && hm_symbol==Other.hm_symbol;
}
    
std::ostream& SymmetryElement::Print(std::ostream& os)const{
        os<<schoenflies_symbol;
        //for(size_t i=0;i<3;++i){for(size_t j=0;j<3;++j)os<<" "<<Elem[i*3+j];os<<std::endl;}
        return os;
}
    
const SymmetryElement Identity({1.0,0.0,0.0,
                                0.0,1.0,0.0,
                                0.0,0.0,1.0},"E","1");    


const SymmetryElement CoI({-1.0,0.0,0.0,
                            0.0,-1.0,0.0,
                            0.0,0.0,-1.0},"i","[1]");
                            
const SymmetryElement Coo({},"C_oo","oo");

inline double ToDegrees(size_t n,size_t m){
    return 360.0/(static_cast<double>(n)/static_cast<double>(m));
}                            

inline std::string SSym(size_t n,size_t m){
    return to_string(n)+(m>1?"^"+to_string(m):"");
}

Rotation::Rotation(const Vector_t& Axis,size_t n,size_t m):
    SymmetryElement(rotation(Axis,ToDegrees(n,m)),"C_"+SSym(n,m),
                    to_string(n)){}                            

MirrorPlane::MirrorPlane(const Vector_t& Norm):
    SymmetryElement(reflection(Norm),"s","m"){}

ImproperRotation::ImproperRotation(const Vector_t& Axis,size_t n,size_t m):
    SymmetryElement(roto_reflection(Axis,ToDegrees(n,m)),"S_"+SSym(n,m),
                    "["+to_string(n%2==1?n*2:(n%4==0?n:n/2))+"]"){}
    
}}//End namespaces


