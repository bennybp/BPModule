/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <array>
#include <unordered_set>
#include <iostream>
#include "pulsar/system/symmetry/Symmetrizer.hpp"
#include "pulsar/system/symmetry/SymmetryElements.hpp"
#include "pulsar/system/System.hpp"
#include "pulsar/math/Point.hpp"
#include "pulsar/math/NumberTheory.hpp"
#include "pulsar/math/BLAS.hpp"
#include "pulsar/math/Checking.hpp"
#include "pulsar/constants.h"
#include "pulsar/exception/Exceptions.hpp"


namespace pulsar{
namespace system{

using pulsar::math::Point;
using pulsar::math::AreEqual;
using pulsar::math::Dot;
using pulsar::math::Cross;
using pulsar::exception::GeneralException;
typedef std::unordered_set<Atom> AtomSet_t;
typedef std::array<double,3> Vector_t;
typedef std::array<double,9> Matrix_t;
typedef std::unordered_set<SymmetryElement> Elems_t;
typedef SymmetryElement SymmEl_t;

//The condition for two atoms being equal
inline bool AtomsAreEqual(const Atom& AI,const Atom& BI){
    return AI.GetZ()==BI.GetZ();
}

/*
 *    Let \f$x\f$ be a unit vector aligned with an axis that passes
 *    through the origin and let \f$p\f$ be a vector from the origin
 *    to some point in space.  The perpendicular distance from
 *    the point and the axis is given by:
 *    \f[
 *       d=|p\times(p-x)|=|\left{p_zy-p_yz,p_xz-p_zx,p_yx-p_xy\right}|
 *    \f]
 *
 */
inline double PerpDist(const Vector_t& Axis,const Atom& Point){
    Vector_t Temp=Cross(Point,Axis);
    return sqrt(Dot(Temp,Temp));
}

//True if manipulated system is symmetrically equivalent to within tol
bool IsGood(const System& Mol,const SymmEl_t& E,const Vector_t& Ax,double Tol){
    System New=Mol.Rotate(E.Elem);
    double MaxDist=-1.0;//The furthest any atom is from its closest partner
    Atom AJ;//Which atom MaxDist goes with
    for(const Atom& AI:Mol){
        double MinDist=1E10;//Closest atom should be closer than a few meters...
        for(const Atom& BI:New)
            MinDist=std::min(AI.Distance(BI),MinDist);
        if(MinDist<MaxDist)continue;
        MaxDist=MinDist;
        AJ=AI;
    }
    double a0=1.0/BOHR_RADIUS_ANGSTROMS,Denom=0.0;
    if(E.SSymbol=="i")Denom=AJ.Magnitude();
    else if(E.HMSymbol=="m")Denom=fabs(Dot(AJ,Ax));
    else Denom=PerpDist(Ax,AJ);
    return(MaxDist/(Denom>a0?Denom:1.0))<Tol;
}

//Returns an arbitrary circular set about a given Axis, Axis must be unit vector
AtomSet_t GetCircularSet(const System& Mol,const Vector_t& Ax,double T){
    AtomSet_t CircularSet;
    for(const Atom& AI:Mol){
        if(PerpDist(Ax,AI)>T){
            double Height=Dot(Ax,AI);
            CircularSet.insert(AI);
            for(const Atom& BI:Mol){
                if(AI==BI||!AtomsAreEqual(AI,BI))continue;
                if(PerpDist(Ax,BI)>T&&AreEqual(Dot(Ax,BI),Height,T))
                    CircularSet.insert(BI);
            }
            break;
        }
    }
    return CircularSet;
}

Vector_t Orient(const Vector_t& Axis){
    //Fix orientation so counter/clockwise are same direction
    //We do this by requiring our axis to point "up" in a right handed
    //coordinate system.  If it lies in the XY plane we then require that
    //the y-coordinate be positive, and if it lies on the x axis the x
    //coordinate need be positive.  To accomplish any of these we need to
    //invert through the origin if the given component is negative
    if(fabs(Axis[2])>1e-2){//Not in XY plane
        if(Axis[2]>0.0)return Vector_t(Axis);//Pointing up already
    }
    else if(fabs(Axis[1])>1e-2){//Not on X-axis
        if(Axis[1]>0.0)return Vector_t(Axis);//Positive Y already
    }
    else{if(Axis[0]>0.0)return Vector_t(Axis);}//Positive X already
    return {-1.0*Axis[0],-1.0*Axis[1],-1.0*Axis[2]};
}

//Looks for Sn along axis Axis to within tolerance T, if found added to Es
void FindSn(const System& M,const Vector_t& Axis,size_t n,double T,Elems_t& Es){
    ImproperRotation Sn2(Axis,2*n,1),Sn(Axis,n,1);
    if(n!=2&&IsGood(M,Sn,Axis,T)){
       const bool Odd=n%2==1;
       size_t Max=(Odd?2*n:n),Self=(Odd?n:n/2),Inc(Odd?2:1);
       for(size_t i=1;i<Max;i+=Inc){
         std::pair<size_t,size_t> Frac=math::Reduce(i,n);
         if(i!=Self)Es.insert(ImproperRotation(Axis,Frac.second,Frac.first));
       }
    }
    if(IsGood(M,Sn2,Axis,T)){
       for(size_t i=1;i<2*n;++i){//Always even
         std::pair<size_t,size_t> Frac=math::Reduce(i,2*n);
         if(i!=n)Es.insert(ImproperRotation(Axis,Frac.second,Frac.first));
       } 
    }
}

//Looks for reflection normal to axis
void FindSigma(const System& Mol,const Vector_t& Axis,double Tol,
               Elems_t& Es){
    Vector_t NewAxis=Orient(Axis);
    MirrorPlane Sigma(NewAxis);
    if(IsGood(Mol,Sigma,NewAxis,Tol))
        Es.insert(Sigma);
}


//Determines if Axis is a Cn to within Tol.  Axis must be unit vector
//Also looks for Sn coincident with Cn and sigma perpendicular to Cn
void FindCn(const System& Mol,const Vector_t& Axis,double Tol,
            Elems_t& Elems){
    Vector_t NewAxis=Orient(Axis);
    AtomSet_t CSet=GetCircularSet(Mol,NewAxis,Tol);
    for(size_t n=CSet.size();n>1;--n){
        Rotation Cn(NewAxis,n,1);
        if(IsGood(Mol,Cn,NewAxis,Tol)){
            for(size_t m=1;m<=n-1;++m){//All n-1 applications of Cn
                std::pair<size_t,size_t> Frac=math::Reduce(m,n);
                Elems.insert(Rotation(NewAxis,Frac.second,Frac.first));
            }
            FindSigma(Mol,NewAxis,Tol,Elems);
            FindSn(Mol,NewAxis,n,Tol,Elems);
            break;
        }
    }//Loop over circular set
}

SymmetryGroup Symmetrizer::GetSymmetry(const System& Mol)const{
    if(Mol.Size()==1)
        return SymmetryGroup({},"Kh","oo/moo");
    
    const double SymTol=0.1;
    Elems_t Elems;
    //RankedGroups FoundGroups;
    Point CoM=Mol.CenterOfMass();
    System CenteredMol=Mol.Translate(-1.0*CoM);
    Matrix_t I=InertiaTensor(CenteredMol);
    Vector_t Moments{0.0,0.0,0.0};
    math::SymmetricDiagonalize(I,Moments);

    //Our molecule aligned with the principal moments of inertia
    System PrinMol=CenteredMol.Rotate(I);
    
    //Taking Trent's percentage based check idea, we'll go with 1% of largest
    double DegenTol=0.01*Moments[2];
    size_t NumDegen=AreEqual(Moments[1],Moments[0],DegenTol)+
            AreEqual(Moments[2],Moments[1],DegenTol)+
            AreEqual(Moments[2],Moments[0],DegenTol);
    //Find center of inversion, if it exists it's at the origin
    if(IsGood(PrinMol,CoI,{0.0,0.0,0.0},SymTol))Elems.insert(CoI);
    
    if(NumDegen==1 && fabs(Moments[0])<SymTol){//Linear molecule
        Elems.insert(SymmetryElement({},"C_oo","oo"));
        return AssignGroup(Elems);
    }
    
    
    //Rotation about principal axes
    for(size_t i=0;i<3;++i){
        Vector_t Axis={0.0,0.0,0.0};
        Axis[i]=1.0;
        const size_t OldSize=Elems.size();
        FindCn(PrinMol,Axis,SymTol,Elems);
        if(Elems.size()==OldSize)continue;
        //Also check parallel to a found rotational axis
        for(size_t j=0;j<3;++j){
            if(i==j)continue;
            Vector_t Axis2={0.0,0.0,0.0};
            Axis2[j]=1.0;
            FindSigma(PrinMol,Axis2,SymTol,Elems);
        }
    }

    //Rotation about atoms
    for(const Atom& AI:PrinMol){
        if(AreEqual(AI[0],0.0,SymTol)&&
           AreEqual(AI[1],0.0,SymTol)&&
           AreEqual(AI[2],0.0,SymTol))continue;//At CoM
        const double mag=AI.Magnitude();
        Vector_t Axis={AI[0]/mag,AI[1]/mag,AI[2]/mag};
        const size_t OldSize=Elems.size();
        if(NumDegen>0)
            FindCn(PrinMol,Axis,SymTol,Elems);
        if(NumDegen!=3&&Elems.size()==OldSize){
            FindSigma(PrinMol,Axis,SymTol,Elems);

        }
    }

    //Now axes at midpoints between like atoms
    //Note: U+V bisects vectors U and V
    for(const Atom& AI:PrinMol){
        for(const Atom& BI:PrinMol){
            //std::cout<<AI<<" "<<BI<<std::endl;
            if(AI==BI)break;
            if(!AtomsAreEqual(AI,BI))continue;
            double CosTheta=
                    Dot(AI,BI)/(AI.Magnitude()*BI.Magnitude());
            if(AreEqual(CosTheta,1.0,SymTol))continue;//Parallel
            if(AreEqual(CosTheta,-1.0,SymTol))continue;//Anti-parallel
            math::Point BiSec=AI+BI;
            double Mag=BiSec.Magnitude();
            Vector_t Axis={BiSec[0]/Mag,BiSec[1]/Mag,BiSec[2]/Mag};
            if(NumDegen>0)
                FindCn(PrinMol,Axis,SymTol,Elems);
            if(NumDegen!=3){//Try plane perp to this
                BiSec=AI-BI;
                Mag=BiSec.Magnitude();
                Axis={BiSec[0]/Mag,BiSec[1]/Mag,BiSec[2]/Mag};
                FindSigma(PrinMol,Axis,SymTol,Elems);
            }
        }
    }
    if(NumDegen==3){
        //Figure out cubic groups...

    }

    std::cout<<AssignGroup(Elems)<<std::endl;
    return AssignGroup(Elems);
}


}
}//End namespaces
