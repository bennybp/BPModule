/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include<cmath>
#include<memory>
#include "pulsar/system/symmetry/SymmetryGroup.hpp"
#include "pulsar/math/BLAS.hpp"
namespace pulsar{
namespace system{

typedef std::unordered_set<SymmetryElement> Elem_t;

bool Contains(const Elem_t& Elems,const std::string& SSym,size_t Mult=1){
    for(const SymmetryElement& Ei: Elems){
        if(Ei.SSymbol==SSym)Mult--;
        if(Mult==0)return true;
    }
    return false;
}

std::array<double,3> GetPrincipleAxis(const Elem_t& Elems,size_t n){
    std::unique_ptr<SymmetryElement> PrincipleAxis;;
    std::string nstr=std::to_string(n);
    for(const auto& Ei: Elems)
        if(Ei.SSymbol==("C_"+nstr)){
            PrincipleAxis=
               std::unique_ptr<SymmetryElement>(new SymmetryElement(Ei));
            break;
        }
    
    NonSymmDiagReturn_t EigenSys=
            math::NonSymmetricDiagonalize(PrincipleAxis->Elem,3);

    return {std::get<1>(EigenSys)[6],
            std::get<1>(EigenSys)[7],
            std::get<1>(EigenSys)[8]};
}

SymmetryGroup AssignGroup(const Elem_t& Elems){
    if(Contains(Elems,"C_oo")){//Find linear groups
        if(Contains(Elems,"i"))
            return SymmetryGroup(Elems,"Dooh","oo/mm");
        return SymmetryGroup(Elems,"Coov","oom");
    }
    if(Contains(Elems,"C_3",2)){//Cubic groups
        if(Contains(Elems,"C_5",2)){
            if(Contains(Elems,"i"))
                return SymmetryGroup(Elems,"Ih","[53]m");
            return SymmetryGroup(Elems,"I","532");
        }
        if(Contains(Elems,"C_4",2)){
            if(Contains(Elems,"i"))
                return SymmetryGroup(Elems,"Oh","m[3]m");
            return SymmetryGroup(Elems,"O","432");
        }
        else if(Contains(Elems,"s")){
            if(Contains(Elems,"i"))
                return SymmetryGroup(Elems,"Th","m[3]");
            return SymmetryGroup(Elems,"Td","[4]3m");
        }
        return SymmetryGroup(Elems,"T","23");
    }
    if(Elems.size()<2){//Low symmetry groups
        if(Contains(Elems,"s"))return SymmetryGroup(Elems,"Cs","m");
        if(Contains(Elems,"i"))return SymmetryGroup(Elems,"Ci","[1]");
        if(Contains(Elems,"C_2"))return SymmetryGroup(Elems,"C2","2");
        return SymmetryGroup(Elems,"C1","1");
    }
    //Find n for the principal axis and establish some strings we'll need
    double MinTheta=360.0;
    for(const auto& Ei: Elems){
        if(Ei.SSymbol.at(0)!='C')continue;
        MinTheta=
          std::min(MinTheta,
                   acos((Ei.Elem[0]+Ei.Elem[4]+Ei.Elem[8]-1.0)/2.0));
    }
    size_t n= (size_t)(std::round(2*3.14159265359/MinTheta));
    const std::string nstr=std::to_string(n);
    const std::string n2=std::to_string(2*n);
    const std::string n2bar="["+n2+"]",nbar="["+nstr+"]";
    
    MirrorPlane Sigmah(GetPrincipleAxis(Elems,n));
    
    if(Contains(Elems,"C_2",n)){
        const std::string D="D"+nstr;
        const bool Even=n%2;
        if(Elems.count(Sigmah)==1)
            return SymmetryGroup(Elems,D+"h",
                   (n>2&&Even?nstr+"/mmm":(n==2?"mmm":n2bar+"m2")));
        if(Contains(Elems,"s"))
            return SymmetryGroup(Elems,D+"d",(Even?n2bar+"2":nbar)+"m");
        return SymmetryGroup(Elems,D,nstr+"2"+(Even?"2":""));
    }
    const std::string C="C"+nstr;
    if(Elems.count(Sigmah)==1)
        return SymmetryGroup(Elems,C+"h",nbar+"/m");
    if(Contains(Elems,"s"))
        return SymmetryGroup(Elems,C+"v",nstr+"m");
    if(Contains(Elems,"S_"+n2))
        return SymmetryGroup(Elems,"S"+n2,nbar);
    return SymmetryGroup(Elems,C,nstr);
}

}}//End namespaces
