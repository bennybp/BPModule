/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "bpmodule/modulebase/EnergyMethod.hpp"
#include "bpmodule/math/FiniteDiff.hpp"
#include "bpmodule/datastore/Wavefunction.hpp"
#include "bpmodule/system/Molecule.hpp"
#include "bpmodule/modulemanager/ModulePtr.hpp"
#include "bpmodule/modulemanager/ModuleManager.hpp"
#include "bpmodule/parallel/InitFinalize.hpp"
#include "LibTaskForce.hpp"

namespace bpmodule {
namespace modulebase {

    typedef std::vector<system::Atom> AtomV_t;
    typedef modulemanager::ModuleManager MM_t;
class FDFunctor:public math::FDiffVisitor<double,std::vector<double>>{
    private:
        size_t Order_;
        const AtomV_t& Atoms_;
        MM_t& MM_;
        std::string Key_;
        unsigned long ID_;
    public:
        using math::FDiffVisitor<double,std::vector<double>>::operator();
        //Returns the i-th Cartesian coordinate of our molecule
        double operator()(size_t i)const{
            return Atoms_[(i-i%3)/3][i%3];
        }
        
        std::vector<double> operator()(size_t i,double newcoord){
            system::AtomSetUniverse NewU;
            size_t atom=(i-i%3)/3;
            for(size_t j=0;j<Atoms_.size();++j){
                NewU<<Atoms_[j];
                if(j==atom)NewU[j][i%3]=newcoord;
            }
            modulemanager::ModulePtr<EnergyMethod> NewMode=
                    MM_.GetModule<EnergyMethod>(Key_,ID_);
            system::Molecule NewMol(NewU,true);
            NewMode->Wfn().system.Set(NewMol);
            return NewMode->Deriv(Order_-1);
            //Make new module
        }
        
        FDFunctor(size_t Order,
                  const AtomV_t& Atoms,
                  MM_t& MM,
                  std::string& Key,
                  unsigned long ID):
            Order_(Order),Atoms_(Atoms),MM_(MM),Key_(Key),ID_(ID){}
};    
    
 
std::vector<double> EnergyMethod::Deriv(size_t Order){
    //if(Order==0)//Throw error
    const system::Molecule& Mol=*Wfn().system;
    //std::vector<system::Atom> Atoms(Mol.begin(),Mol.end());
    const LibTaskForce::Communicator& Comm=parallel::GetEnv().Comm();
    std::cout<<Comm<<std::endl;
    std::cout<<Mol<<std::endl;
    
    /*math::CentralDiff<double,std::vector<double>> FD(Comm.Split());
    FDFunctor Thing2Run=FDFunctor(Order,Atoms,MManager(),ID(),Key());
    std::vector<std::vector<double>> TempDeriv=
    FD.Run(Thing2Run,Mol.NAtoms(),0.02,3);
    //Need to flatten the array, abuse that TempDeriv[0] is the first deriv comp    
    for(size_t i=1;i<TempDeriv.size();++i)
       for(double j :  TempDeriv[i])TempDeriv[0].push_back(j);
    return TempDeriv[0];*/
    return std::vector<double>(2);
}
    
}}
