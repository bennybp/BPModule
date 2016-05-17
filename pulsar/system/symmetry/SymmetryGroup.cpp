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
        std::pair<size_t,size_t> Frac=math::Reduce(i,n);
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
        std::pair<size_t,size_t> Frac=math::Reduce(i,n);
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
        std::set<size_t> Fs=math::Factors(n);
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
                  Elems(AddE(Es)),SSymbol(SSym),HMSymbol(HMSym){
}

bool SymmetryGroup::Finite()const{
    return (SSymbol!="Kh"&&SSymbol!="Coov"&&SSymbol!="Dooh");
}

size_t SymmetryGroup::Order()const{
    if(!Finite())
        throw exception::GeneralException("Group is infinite order");
    return Elems.size();
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
    SymmetryGroup(Es,"S"+to_string(n*2),"["+to_string(n)+"]"){}
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

}//End PointGroup namespace

bool Contains(const Elem_t& Elems,const std::string& SSym,size_t Mult=1){
    for(const SymmetryElement& Ei: Elems){
        if(Ei.SSymbol==SSym)Mult--;
        if(Mult==0)return true;
    }
    return false;
}

std::array<double,3> GetPrincipleAxis(const Elem_t& Elems,size_t n){
    typedef std::unique_ptr<SymmetryElement> pElem_t;
    using std::get;
    pElem_t PrincipleAxis;;
    std::string nstr=to_string(n);
    for(const auto& Ei: Elems)
        if(Ei.SSymbol==("C_"+nstr)){
            PrincipleAxis=pElem_t(new SymmetryElement(Ei));
            break;
        }
    NonSymmDiagReturn_t EigenSys=
            math::NonSymmetricDiagonalize(PrincipleAxis->Elem,3);
    return {get<1>(EigenSys)[6],get<1>(EigenSys)[7],get<1>(EigenSys)[8]};
}

SymmetryGroup AssignGroup(const Elem_t& Elems){
    using namespace PointGroup;
    if(Contains(Elems,"C_oo")){//Find linear groups
        if(Contains(Elems,"i"))return Dooh(Elems);
        return Coov(Elems);
    }
    if(Contains(Elems,"C_3",2)){//Cubic groups
        if(Contains(Elems,"C_5",2)){
            if(Contains(Elems,"i"))return Ih(Elems);
            return I(Elems);
        }
        if(Contains(Elems,"C_4",2)){
            if(Contains(Elems,"i"))return Oh(Elems);
            return O(Elems);
        }
        if(Contains(Elems,"s")){
            if(Contains(Elems,"i")) return Th(Elems);
            return Td(Elems);
        }
        return T(Elems);
    }
    if(Elems.size()<2){//Low symmetry groups
        if(Contains(Elems,"s"))return Cs(Elems);
        if(Contains(Elems,"i"))return Ci(Elems);
        if(Contains(Elems,"C_2"))return Cn(Elems,2);
        return Cn(Elems,1);
    }
    //Find n for the principal axis and establish some strings we'll need
    double MinTheta=360.0;
    for(const auto& Ei: Elems){
        if(Ei.SSymbol.at(0)!='C')continue;
        MinTheta=
          std::min(MinTheta,
                   acos((Ei.Elem[0]+Ei.Elem[4]+Ei.Elem[8]-1.0)/2.0));
    }
    size_t n= (size_t)(std::round(2*PI/MinTheta));
    MirrorPlane Sigmah(GetPrincipleAxis(Elems,n)); 
    if(Contains(Elems,"C_2",n)){
        if(Elems.count(Sigmah)==1)return Dnh(Elems,n);
        if(Contains(Elems,"s"))return Dnd(Elems,n);
        return Dn(Elems,n);
    }
    if(Elems.count(Sigmah)==1)return Cnh(Elems,n);
    if(Contains(Elems,"s"))return Cnv(Elems,n);
    if(Contains(Elems,"S_"+to_string(2*n)))return Sn(Elems,n);
    return Cn(Elems,n);
}

}}//End namespaces
