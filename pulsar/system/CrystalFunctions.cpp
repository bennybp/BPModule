/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <cmath>
#include <algorithm>
#include <array>
#include "pulsar/system/CrystalFunctions.hpp"
#include "pulsar/util/IterTools.hpp"
#include "pulsar/math/CombItr.hpp"
#include "pulsar/constants.h"
#include "pulsar/system/Space.hpp"


typedef std::array<double,3> Vector_t;

namespace pulsar{
namespace system{

void ToRadian(double& angle){
    angle*=PI/180.0;
}

AtomSetUniverse Frac2Cart(const AtomSetUniverse& DaU,const Space& DaSpace){
    Vector_t CosA,Angles(DaSpace.LatticeAngles);
    const Vector_t& Sides=DaSpace.LatticeSides;
    std::for_each(Angles.begin(),Angles.end(),ToRadian);                 
    std::transform(Angles.begin(),Angles.end(),CosA.begin(),cos);
    double CosBG=CosA[1]*CosA[2],SinG=sin(Angles[2]);
    double v=sqrt(1-CosA[0]*CosA[0]-CosA[1]*CosA[1]-CosA[2]*CosA[2]
                  -2*CosA[0]*CosBG);
    System DaSys(DaU,true);
    System Temp=DaSys.Rotate(std::array<double,9>({Sides[0],Sides[1]*CosA[2],Sides[2]*CosA[1],
                             0,Sides[2]*SinG,Sides[2]*(CosA[0]-CosBG)/SinG,
                             0,0,Sides[2]*v/SinG}));
    return Temp.AsUniverse()-DaU;
}

AtomSetUniverse MakeSuperCell(const AtomSetUniverse& DaU,
                     const std::array<size_t,3>& Dims,const Vector_t& Sides){
    AtomSetUniverse NewU;
    System UC(DaU,true);
    for(size_t x=0;x<Dims[0];++x){
        const double xSide=x*Sides[0];
        for(size_t y=0;y<Dims[1];++y){
            const double ySide=y*Sides[1];
            for(size_t z=0;z<Dims[2];++z)
                NewU+=UC.Translate(Vector_t({xSide,ySide,z*Sides[2]})).AsUniverse();
        }
    }
    return NewU;
}

void Recursion(const Conn_t& Conns,const Atom& atomI,AtomSetUniverse& NewU){
    for(const Atom& atomJ: Conns.at(atomI))
        if(!NewU.Contains(atomJ)){
            NewU.Insert(atomJ);
            Recursion(Conns,atomJ,NewU);
        }
}


AtomSetUniverse CarveUC(const AtomSetUniverse& SC,
                        const  Vector_t& Sides,
                        double MinScale,
                        double MaxScale){
    AtomSetUniverse NewU;
    Conn_t Conns=GetConns(System(SC,true));
    std::array<double,3> Low,High;
    std::transform(Sides.begin(),Sides.end(),Low.begin(),
                   [MinScale](const double& a){return a*MinScale;});
    std::transform(Sides.begin(),Sides.end(),High.begin(),
                    [MaxScale](const double& a){return a*MaxScale;});
    for(auto& ConnI:Conns){
        if(NewU.Contains(ConnI.first))continue;
        bool InCell=true;
        for(size_t x: util::Range<0,3>())
            if(ConnI.first[x]<Low[x]||ConnI.first[x]>High[x]){
                InCell=false;
                break;
            }
        if(!InCell) continue;
        NewU.Insert(ConnI.first);
        Recursion(Conns,ConnI.first,NewU);
    }            
    Vector_t Trans;
    std::transform(Sides.begin(),Sides.end(),Trans.begin(),
                   [](const double& a){return a*-1.0;});
    return System(NewU,true).Translate(Trans).AsUniverse();              
}

AtomSetUniverse CleanUC(const AtomSetUniverse& UC, 
                        const std::array<double,3>& Sides){
    AtomSetUniverse NewUC,SC=MakeSuperCell(UC,{3,3,3},Sides);
    NewUC=CarveUC(SC,Sides);
    std::cout<<ToAngstroms(System(NewUC,true))<<std::endl;
            exit(1);
    while(true){
        size_t OldSize=NewUC.size();
        SC=MakeSuperCell(NewUC,{3,3,3},Sides);
        math::CombItr<AtomSetUniverse> Pairs(SC,2);
        while(Pairs){
            std::cout<<(*Pairs)[0]<<" "<<(*Pairs)[1]<<std::endl;
            if((*Pairs)[0]==(*Pairs)[1])
                std::cout<<(*Pairs)[0]<<(*Pairs)[1]<<std::endl;
            ++Pairs;
        }
        exit(1);
        
    }
}

}}//End namespaces
