/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include<cmath>
#include<memory>
#include "pulsar/system/symmetry/SymmetryGroup.hpp"
#include "pulsar/math/BLAS.hpp"
#include "pulsar/math/NumberTheory.hpp"
#include "pulsar/math/Checking.hpp"
#include "pulsar/constants.h"
#include "pulsar/exception/Exceptions.hpp"

namespace pulsar{
namespace system{

using std::to_string;
typedef std::unordered_set<SymmetryElement> Elem_t;



inline std::string DnhMHSym(size_t n){
    if(n==2)return "mmm";
    if(n%2==0)return to_string(n)+"/mmm";        
    return "["+to_string(2*n)+"]m2";
}

inline Elem_t GenCnElems(size_t n){
    Elem_t Results;
    for(size_t i=1;i<n;++i){
        std::pair<size_t,size_t> Frac=reduce(i,n);
        Results.insert(Rotation({0.0,0.0,1.0},Frac.second,Frac.first));
    }
    return Results;
}

inline Elem_t GenCnvElems(size_t n){
    Elem_t Results=GenCnElems(n);
    //The n planes are all vertical and separated by an angle of 180/n
    const double theta=PI/n;
    for(size_t i=0;i<n;++i)//Oooh..I get to use polar coordinates
        Results.insert(MirrorPlane({cos(i*theta),sin(i*theta),0.0}));
    return Results;
}

inline Elem_t GenSnElems(size_t n){
    const bool Even=n%2==0;
    Elem_t Results=GenCnElems(Even?n/2:n);
    const size_t Max=(Even?n:2*n);
    for(size_t i=1;i<Max;i+=2){
        std::pair<size_t,size_t> Frac=reduce(i,n);
        if(Frac==std::make_pair<size_t,size_t>(1,2))Results.insert(CoI);
        else if(Frac==std::make_pair<size_t,size_t>(1,1))
            Results.insert(MirrorPlane({0.0,0.0,1.0}));
        else Results.insert(ImproperRotation({0.0,0.0,1.0},
                                          Frac.second,Frac.first));
    }
    return Results;
}

inline Elem_t GenCnhElems(size_t n){
    Elem_t Results=GenSnElems(n);
    if(n%2==0){//Have to manually get other S groups and Cn
        std::set<size_t> Fs=factors(n);
        for(size_t Fi:Fs){
            Elem_t Temp=GenSnElems(Fi);
            Results.insert(Temp.begin(),Temp.end());
        }
        Elem_t Temp=GenCnElems(n);
        Results.insert(Temp.begin(),Temp.end());
    }
    return Results;
    
}

inline Elem_t GenDnElems(size_t n){
    Elem_t Results=GenCnElems(n);
    //There are n Cn axis perpendicular to Z
    const double theta=PI/n;
    for(size_t i=0;i<n;++i)
        Results.insert(Rotation({cos(i*theta),sin(i*theta),0.0},2,1));
    return Results;
}

inline Elem_t GenDnhElems(size_t n){
    Elem_t Results=GenCnhElems(n);
    const double theta=PI/n;
    for(size_t i=0;i<n;++i){
        Results.insert(Rotation({cos(i*theta),sin(i*theta),0.0},2,1));
        Results.insert(MirrorPlane({cos(i*theta),sin(i*theta),0.0}));
    }
    Results.insert(MirrorPlane({0.0,0.0,1.0}));
    return Results;
}

inline Elem_t GenDndElems(size_t n){
    Elem_t Results=GenSnElems(n*2);
    const double theta=PI/n;
    for(size_t i=0;i<n;++i){
        Results.insert(Rotation({cos(i*theta),sin(i*theta),0.0},2,1));
        Results.insert(MirrorPlane({cos(i*theta),sin(i*theta),0.0}));
    }
    return Results;
}

inline Elem_t AddE(const Elem_t& Es){
    Elem_t Temp(Es);
    Temp.insert(Identity);
    return Temp;
}

SymmetryGroup::SymmetryGroup(const Elem_t& Es,
                  const std::string SSym,const std::string HMSym):
                  symmetry_elements(AddE(Es)),schoenflies_symbol(SSym),hm_symbol(HMSym){
}

bool SymmetryGroup::finite()const{
    return (schoenflies_symbol!="Kh"&&schoenflies_symbol!="Coov"&&schoenflies_symbol!="Dooh");
}

size_t SymmetryGroup::order()const{
    if(!finite())
        throw GeneralException("Group is infinite order");
    return symmetry_elements.size();
}

namespace PointGroup{
Ci::Ci():Ci({CoI}){}

Cs::Cs():Cs({MirrorPlane({1.0,0.0,0.0})}){}

Cn::Cn(const Elem_t& Es,size_t n):
        SymmetryGroup(Es,"C"+to_string(n),to_string(n)){}
Cn::Cn(size_t n):Cn(GenCnElems(n),n){}

Cnv::Cnv(const Elem_t& Es,size_t n):
        SymmetryGroup(Es,"C"+to_string(n)+"v",to_string(n)+"m"){}
Cnv::Cnv(size_t n):Cnv(GenCnvElems(n),n){}

Cnh::Cnh(const Elem_t& Es,size_t n):
        SymmetryGroup(Es,"C"+to_string(n)+"h","["+to_string(n)+"]/m"){}
Cnh::Cnh(size_t n):Cnh(GenCnhElems(n),n){}

Sn::Sn(const Elem_t& Es,size_t n):
    SymmetryGroup(Es,"S"+to_string(n),"["+to_string(n)+"]"){}

Sn::Sn(size_t n):Sn(GenSnElems(n),n){}

Dn::Dn(const Elem_t& Es,size_t n):
    SymmetryGroup(Es,"D"+to_string(n),to_string(n)+"2"+(n%2==0?"2":"")){}
Dn::Dn(size_t n):Dn(GenDnElems(n),n){}

Dnh::Dnh(const Elem_t& Es,size_t n):
    SymmetryGroup(Es,"D"+to_string(n)+"h",DnhMHSym(n)){}
Dnh::Dnh(size_t n):Dnh(GenDnhElems(n),n){}

Dnd::Dnd(const Elem_t& Es,size_t n):
    SymmetryGroup(Es,"D"+to_string(n)+"d",
                  (n%2==0?"["+to_string(n*2)+"]2":"["+to_string(n)+"]")+"m"){}
Dnd::Dnd(size_t n):Dnd(GenDndElems(n),n){}

Coov::Coov():Coov({Coo}){}

Dooh::Dooh():Dooh({Coo,CoI}){}

}//End PointGroup namespace

bool contains(const Elem_t& Elems,const std::string& SSym,size_t Mult=1){
    for(const SymmetryElement& Ei: Elems){
        if(Ei.schoenflies_symbol==SSym)Mult--;
        if(Mult==0)return true;
    }
    return false;
}

std::array<double,3> get_principle_axis(const Elem_t& Elems,size_t n){
    typedef std::unique_ptr<SymmetryElement> pElem_t;
    using std::get;
    pElem_t PrincipleAxis;;
    std::string nstr=to_string(n);
    for(const auto& Ei: Elems)
        if(Ei.schoenflies_symbol==("C_"+nstr)){
            PrincipleAxis=pElem_t(new SymmetryElement(Ei));
            break;
        }
    //Want right eigenvector with eigenvalue 1
    NonSymmDiagReturn_t EigenSys=
            NonSymmetricDiagonalize(PrincipleAxis->element_matrix,3);
    size_t Axis=2;//Will usually be the last eigenvector, but not always
    std::vector<std::complex<double>> Evs=get<0>(EigenSys);
    for(size_t i=3;i>0;--i){
        if(are_equal(Evs[i-1].real(),1.0,1e-3)&&
           are_equal(Evs[i-1].imag(),0.0,1e-3)){
            Axis=i-1;
            break;
        }
    }
    std::vector<double> Evec=get<1>(EigenSys);
    return {Evec[Axis*3],Evec[Axis*3+1],Evec[Axis*3+2]};
}

SymmetryGroup assign_group(const Elem_t& Elems){
    using namespace PointGroup;
    if(contains(Elems,"C_oo")){//Find linear groups
        if(contains(Elems,"i"))return Dooh(Elems);
        return Coov(Elems);
    }
    if(contains(Elems,"C_3",2)){//Cubic groups
        if(contains(Elems,"C_5",2)){
            if(contains(Elems,"i"))return Ih(Elems);
            return I(Elems);
        }
        if(contains(Elems,"C_4",2)){
            if(contains(Elems,"i"))return Oh(Elems);
            return O(Elems);
        }
        if(contains(Elems,"s")){
            if(contains(Elems,"i")) return Th(Elems);
            return Td(Elems);
        }
        return T(Elems);
    }
    if(Elems.size()<2){//Low symmetry groups
        if(contains(Elems,"s"))return Cs(Elems);
        if(contains(Elems,"i"))return Ci(Elems);
        if(contains(Elems,"C_2"))return Cn(Elems,2);
        return Cn(Elems,1);
    }
    //Find n for the principal axis and establish some strings we'll need
    double MinTheta=360.0;
    for(const auto& Ei: Elems){
        if(Ei.schoenflies_symbol.at(0)!='C')continue;
        MinTheta=
          std::min(MinTheta,
                   acos((Ei.element_matrix[0]+Ei.element_matrix[4]+Ei.element_matrix[8]-1.0)/2.0));
    }
    size_t n= (size_t)(std::round(2*PI/MinTheta));
    if(n==0)throw GeneralException(
       "Your symmetry elements do not constitute a point group");
    MirrorPlane Sigmah(get_principle_axis(Elems,n)); 
    if(contains(Elems,"C_2",n)){
        if(Elems.count(Sigmah)==1)return Dnh(Elems,n);
        if(contains(Elems,"s"))return Dnd(Elems,n);
        return Dn(Elems,n);
    }
    if(Elems.count(Sigmah)==1)return Cnh(Elems,n);
    if(contains(Elems,"s"))return Cnv(Elems,n);
    if(contains(Elems,"S_"+to_string(2*n)))return Sn(Elems,2*n);
    return Cn(Elems,n);
}

}}//End namespaces
