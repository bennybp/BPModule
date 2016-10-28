/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <array>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include "pulsar/system/symmetry/Symmetrizer.hpp"
#include "pulsar/system/symmetry/SymmetryElement.hpp"
#include "pulsar/system/Atom.hpp"
#include "pulsar/system/System.hpp"
#include "pulsar/math/CombItr.hpp"
#include "pulsar/math/Point.hpp"
#include "pulsar/math/Geometry.hpp"
#include "pulsar/math/NumberTheory.hpp"
#include "pulsar/math/BLAS.hpp"
#include "pulsar/math/Checking.hpp"
#include "pulsar/constants.h"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/math/CombItr.hpp"


namespace pulsar{
namespace system{

using std::make_pair;
using pulsar::Point;
using pulsar::are_equal;
using pulsar::Dot;
using pulsar::Cross;
using pulsar::GeneralException;
typedef std::unordered_set<Atom> AtomSet_t;
typedef std::array<double,3> Vector_t;
typedef std::array<double,9> Matrix_t;
typedef std::unordered_set<SymmetryElement> Elems_t;
typedef SymmetryElement SymmEl_t;
typedef std::vector<std::vector<Atom> > SEASet_t;
typedef std::tuple<System,Vector_t,Matrix_t> MoISys_t;

//The condition for two atoms being equal

inline bool atoms_are_equal(const Atom& AI,const Atom& BI){
    return AI.Z==BI.Z;
}

//True if manipulated system is symmetrically equivalent to within tol

bool IsGood(const System& Mol,const SymmEl_t& E,const Vector_t& Ax,double Tol){
    System New=Mol.rotate(E.element_matrix);
    double MaxDist=-1.0;//The furthest any atom is from its closest partner
    Atom AJ;//Which atom MaxDist goes with
    for(const Atom& AI:Mol){
        double MinDist=1E10;//Closest atom should be closer than a few meters...
        for(const Atom& BI:New)
            MinDist=std::min(AI.distance(BI),MinDist);
        if(MinDist<MaxDist)continue;
        MaxDist=MinDist;
        AJ=AI;
    }
    double a0=1.0/BOHR_RADIUS_ANGSTROMS,Denom=0.0;
    if(E.schoenflies_symbol=="i")Denom=AJ.magnitude();
    else if(E.hm_symbol=="m")Denom=fabs(Dot(AJ,Ax));
    else{//Use perpindicular distance
        Vector_t Temp=Cross(AJ,Ax);
        Denom=sqrt(Dot(Temp,Temp));
    }
    return(MaxDist/(Denom>a0?Denom:1.0))<Tol;
}

template<typename T>
Vector_t Orient(const T& Axis){
    Vector_t NAxis=Normalize(Axis);
    //Fix orientation so counter/clockwise are same direction
    //We do this by requiring our axis to point "up" in a right handed
    //coordinate system.  If it lies in the XY plane we then require that
    //the y-coordinate be positive, and if it lies on the x axis the x
    //coordinate need be positive.  To accomplish any of these we need to
    //invert through the origin if the given component is negative
    if(fabs(NAxis[2])>1e-2){//Not in XY plane
        if(NAxis[2]>0.0)return Vector_t(NAxis);//Pointing up already
    }
    else if(fabs(NAxis[1])>1e-2){//Not on X-axis
        if(NAxis[1]>0.0)return Vector_t(NAxis);//Positive Y already
    }
    else{
        if(NAxis[0]>0.0)return Vector_t(NAxis);
    }//Positive X already
    return
    {
        -1.0*NAxis[0],-1.0*NAxis[1],-1.0*NAxis[2]
    };
}

///Finds all symmetry equivalent atoms

SEASet_t FindSEAs(const System& Mol,double Tol){
    DistMat_t DistMat=pulsar::get_distance(Mol);
    SEASet_t SEAs;
    //Little class to enforce equality to within a tolerance

    struct AreLess{
        double Tol;

        bool operator()(double LHS,double RHS)const{
            return LHS<RHS && !are_equal(LHS,RHS,Tol);
        }
    } Comparer;

    Comparer.Tol=Tol;
    std::unordered_map<Atom,bool> IsGood;
    for(Atom atomI:Mol)IsGood.emplace(atomI,true);
    System::const_iterator atomI=Mol.begin(),atomK=Mol.begin();

    for(;atomI!=Mol.end();++atomI){//Loop over unassigned atoms
        if(!IsGood.at(*atomI))continue;
        SEAs.push_back(std::vector<Atom>({*atomI}));
        IsGood.at(*atomI)=false;
        std::multiset<double,AreLess> ColI(Comparer);
        for(const Atom atomJ:Mol){
            if((*atomI)==atomJ)continue;
            ColI.insert(DistMat[{*atomI,atomJ}
            ]);
        }
        atomK=atomI;
        for(++atomK;atomK!=Mol.end();++atomK){
            if(!IsGood.at(*atomK)||!atoms_are_equal(*atomI,*atomK))continue;
            std::multiset<double,AreLess> CopyColI(ColI);
            for(const Atom atomJ:Mol){
                if(*atomK==atomJ)continue;
                double dist=DistMat[{*atomK,atomJ}
                ];
                auto Location=CopyColI.find(dist);
                if(Location==CopyColI.end())break;
                CopyColI.erase(Location);
            }
            if(CopyColI.size()==0){
                SEAs.back().push_back(*atomK);
                IsGood.at(*atomK)=false;
            }
        }
    }
    return SEAs;
}

//System Symmetrizer::Symmetrize(const System& Mol,double Tol)const{
//    
//}

MoISys_t MoIOrient(const System& Mol){
    Point CoM=Mol.center_of_mass();
    System CenteredMol=Mol.translate(-1.0*CoM);
    Matrix_t I=inertia_tensor(CenteredMol);
    Vector_t Moments{0.0,0.0,0.0};
    SymmetricDiagonalize(I,Moments);
    //Our molecule aligned with the principal moments of inertia
    //System PrinMol=CenteredMol.rotate(I);
    return std::make_tuple(CenteredMol,Moments,I);
}

size_t NDegen(const Vector_t& Moments){
    //Taking Trent's percentage based check idea, we'll go with 5% of largest
    double DegenTol=0.05*Moments[2];
    return are_equal(Moments[1],Moments[0],DegenTol)+
            are_equal(Moments[2],Moments[1],DegenTol)+
            are_equal(Moments[2],Moments[0],DegenTol);
}

Vector_t Transform(const Vector_t& Old,
                   const System& OldSys,const System& NewSys){
    Point Shift=NewSys.center_of_mass()-OldSys.center_of_mass();
    return {Old[0]+Shift[0],Old[1]+Shift[1],Old[2]+Shift[2]};
}

typedef std::pair<Vector_t,size_t> Axis_t;

inline bool IsPlanar(const Vector_t& Moments){
    return are_equal(Moments[0]+Moments[1],Moments[2],0.05*Moments[2]);
}

std::set<Axis_t> NewGetCns(const System& PrinMol,const SEASet_t& SEAs,double Tol){

    //We are going to queue up a set of possible rotation axes
    std::set<Axis_t> Axes;
    for(const std::vector<Atom>& SEA:SEAs){
        if(SEA.size()==1)continue;//No info there...
        CombItr<std::vector<Atom>>AtomPair(SEA,2);
        while(AtomPair){
            //Condition A for finding C2s
            Point BiSec=(*AtomPair)[0]+(*AtomPair)[1];
            if(!are_equal(BiSec,0.0,Tol))
                Axes.insert(make_pair(Orient(BiSec),2));
            ++AtomPair;
            if(SEA.size()!=2)continue;
            //Condition C for finding C2s
            for(const std::vector<Atom>& SEA2:SEAs){
                if(SEA2.size()!=2)continue;
                if(SEA2==SEA)break;
                Axes.insert(
                  make_pair(Orient(Cross(SEA[1]-SEA[0],SEA2[1]-SEA2[0])),2)
                );
            }
        }
        if(SEA.size()==2)continue;


        System NSEA(PrinMol.as_universe(),false);
        for(const Atom atomI:SEA){
            //Condition B for finding C2s
            Axes.insert(std::make_pair(Orient(atomI),2));
            NSEA.insert(atomI);
        }
        MoISys_t PrinSEA=MoIOrient(NSEA);
        System NewSEA=std::get<0>(PrinSEA);
        Vector_t SEAMoms=std::get<1>(PrinSEA);
        if(IsPlanar(SEAMoms)){
            Vector_t PlaneNorm=Orient(get_plane(SEA[0],SEA[1],SEA[2]));
            Axes.insert(make_pair(PlaneNorm,SEA.size()));
        }
        else{
            Matrix_t Is=std::get<2>(PrinSEA);
            size_t max=3,min=0,k=SEA.size()/2;
            double MomTol=0.05*SEAMoms[2];
            if(are_equal(SEAMoms[0],SEAMoms[1],MomTol))min=2;
            else if(are_equal(SEAMoms[1],SEAMoms[2],MomTol))max=1;
            else k=2;
            for(size_t i=min;i<max;++i){
                Vector_t Temp={Is[i*3],Is[i*3+1],Is[i*3+2]};
                Axes.insert(make_pair(Orient(Transform(Temp,NewSEA,PrinMol)),k));
            }
        }

    }
    return Axes;
}

inline void FindCoI(const System& Mol,Elems_t& Elems,double Tol){
    if(IsGood(Mol,CoI,{0.0,0.0,0.0},Tol))
        Elems.insert(CoI);
}

//The check for linearity based off of moments of inertia

inline bool IsLinear(const Vector_t& Moments){
    size_t NumDegen=NDegen(Moments);
    return(NumDegen==1&&are_equal(Moments[0],0.0,1.0E-3));
}

//Looks for Cn and if found repeatedly applies it

void CheckCn(int n,const System& Mol,const Vector_t& Axis,
             double Tol,Elems_t& Elems){
    Rotation Cm(Axis,n,1);
    if(!IsGood(Mol,Cm,Axis,Tol))return;
    Elems.insert(Cm);
    for(int i=2;i<n;++i)
        if(relatively_prime(n,i))
            Elems.insert(Rotation(Axis,n,i));
}

//Looks for Sn and if found repeatedly applies it and adds to the Elems

void CheckSn(size_t n,const System& Mol,const Vector_t& Axis,
             double Tol,Elems_t& Elems){
    if(n<3)return;
    ImproperRotation Sm(Axis,n,1);
    if(!IsGood(Mol,Sm,Axis,Tol))return;
    Elems.insert(Sm);
    const bool Odd=n%2==1;
    const size_t Max=(Odd?2*n:n);
    for(size_t i=3;i<Max;i+=2){
        std::pair<size_t,size_t> Frac=reduce(i,n);
        if(Frac==std::make_pair<size_t,size_t>(1,1))continue;//Sn^n
        if(Frac==std::make_pair<size_t,size_t>(1,2))continue;//S2
        Elems.insert(ImproperRotation(Axis,Frac.second,Frac.first));
    }
}

SymmetryGroup Symmetrizer::get_symmetry(const System& Mol)const{
    if(Mol.size()==1)
        return PointGroup::Kh(std::unordered_set<SymmetryElement>());
    const double SymTol=0.1;


    MoISys_t PMol=MoIOrient(Mol);
    System PrinMol=std::get<0>(PMol);
    Vector_t Moments=std::get<1>(PMol);
    Elems_t Elems;

    //Find center of inversion 1st b/c linear may have
    FindCoI(PrinMol,Elems,SymTol);

    if(IsLinear(Moments)){//Linear molecule
        Elems.insert(Coo);
        return assign_group(Elems);
    }

    //If it's planar that plane is a symmetry element
    if(IsPlanar(Moments)){
        Atom atomI;
        for(const Atom& atomJ: PrinMol)//Can't be at origin
            if(!are_equal(atomJ,0.0,SymTol)){
                atomI=atomJ;
                break;
            }
        Vector_t NatomI=Normalize(atomI);
        for(const Atom atomJ:PrinMol){
            if(atomI==atomJ)continue;
            if(are_equal(atomJ,0.0,SymTol))continue;
            Vector_t NatomJ=Normalize(atomJ);
            if(!are_equal(fabs(Dot(NatomI,NatomJ)),1.0,0.001)){//Not linear
                Vector_t Norm=Orient(Cross(NatomI,NatomJ));
                MirrorPlane Sigma(Norm);
                if(IsGood(PrinMol,Sigma,Norm,SymTol))
                    Elems.insert(Sigma);
                break;
            }
        }
    }

    //Determine symmetry equivalaent atoms SEAs
    SEASet_t SEAs=FindSEAs(PrinMol,SymTol);

    if(SEAs.size()==PrinMol.size())
        return assign_group(Elems);

    std::set<Axis_t> Axes=NewGetCns(PrinMol,SEAs,SymTol);

    for(const Axis_t& axis:Axes){
        for(int k:factors((int)axis.second)){
            if(k==1)continue;
            Rotation Cm(axis.first,k,1);
            size_t Old=Elems.size();
            CheckCn(k,PrinMol,axis.first,SymTol,Elems);
            if(Elems.size()!=Old){
                CheckSn(k,PrinMol,axis.first,SymTol,Elems);
                CheckSn(2*k,PrinMol,axis.first,SymTol,Elems);
            }
        }
    }
    std::set<Vector_t> Planes;
    for(std::vector<Atom> SEA:SEAs){
        if(SEA.size()<2)continue;
        CombItr<std::vector<Atom>>AtomPairs(SEA,2);
        while(AtomPairs){
            Vector_t Norm=Normalize((*AtomPairs)[0]-(*AtomPairs)[1]);
            Planes.insert(Orient(Norm));
            ++AtomPairs;
        }
    }
    
    for(Vector_t Norm: Planes){
        MirrorPlane Sigma(Norm);
        if(IsGood(PrinMol,Sigma,Norm,SymTol))
            Elems.insert(Sigma);
    }

    //for(const auto& i:Elems)std::cout<<i<<" ";
    //std::cout<<std::endl;
    std::cout<<assign_group(Elems)<<std::endl;

    return assign_group(Elems);
}


}
}//End namespaces
