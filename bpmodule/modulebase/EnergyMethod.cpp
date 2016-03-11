/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "bpmodule/modulebase/EnergyMethod.hpp"
#include "bpmodule/math/FiniteDiff.hpp"
#include "bpmodule/datastore/Wavefunction.hpp"
#include "bpmodule/system/System.hpp"
#include "bpmodule/modulemanager/ModulePtr.hpp"
#include "bpmodule/modulemanager/ModuleManager.hpp"
#include "bpmodule/parallel/InitFinalize.hpp"
#include "LibTaskForce.hpp"

namespace bpmodule {
namespace modulebase {

    using system::Atom;
    typedef std::vector<Atom> AtomV_t;
    typedef modulemanager::ModuleManager MM_t;
    typedef std::vector<double> Return_t;
    using system::System;
    using system::AtomSetUniverse;
    using LibTaskForce::Communicator;
    
class FDFunctor:public math::FDiffVisitor<double,Return_t>{
    private:
        typedef  math::FDiffVisitor<double,Return_t> Base_t;
        typedef modulemanager::ModulePtr<EnergyMethod> Module_t;
        size_t Order_;
        const AtomV_t& Atoms_;
        MM_t& MM_;
        std::string Key_;
        unsigned long ID_;
    public:
        //Base class operators are fine in all but two cases
        using Base_t::operator();
        
        //Returns the i-th Cartesian coordinate of our molecule
        double operator()(size_t i)const{return Atoms_[(i-i%3)/3][i%3];}
        
        Return_t operator()(size_t i,const double& newcoord)const{
            AtomSetUniverse NewU;
            for(size_t j=0;j<Atoms_.size();++j){
                NewU<<Atoms_[j];
                if(j==(i-i%3)/3)NewU[j][i%3]=newcoord;
            }
            Module_t NewModule=MM_.GetModule<EnergyMethod>(Key_,ID_);
            NewModule->Wfn().system.Set(System(NewU,true));
            return NewModule->Deriv(Order_-1);
        }
        
        FDFunctor(size_t Order,const AtomV_t& Atoms,
                  MM_t& MM,std::string Key,unsigned long ID):
            Order_(Order),Atoms_(Atoms),MM_(MM),Key_(Key),ID_(ID){}
};    
    
 
Return_t EnergyMethod::Deriv_(size_t Order){
    //if(Order==0)//Throw error
    const System& Mol=*Wfn().system;
    std::vector<Atom> Atoms;
    std::vector<Return_t> TempDeriv;
    //I don't know why the fill constructor is not working...
    for(const Atom& AnAtom: Mol)
          Atoms.push_back(AnAtom);
    const Communicator& Comm=parallel::GetEnv().Comm();
    Communicator NewComm=Comm.Split(1,1);
    
    math::CentralDiff<double,Return_t> FD(NewComm);

    FDFunctor Thing2Run=FDFunctor(Order,Atoms,MManager(),Key(),ID());
    TempDeriv=FD.Run(Thing2Run,3*Mol.NAtoms(),0.02,3);
    //Flatten the array & abuse fact that TempDeriv[0] is the first comp    
    for(size_t i=1;i<TempDeriv.size();++i)
       for(double j :  TempDeriv[i])
           TempDeriv[0].push_back(j);

    return TempDeriv[0];
}
    
}}
