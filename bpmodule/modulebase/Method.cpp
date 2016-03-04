/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "bpmodule/modulebase/Method.hpp"
//#include "bpmodule/math/FiniteDiff.hpp"
#include "bpmodule/datastore/Wavefunction.hpp"
#include "bpmodule/system/Molecule.hpp"
#include "bpmodule/modulemanager/ModulePtr.hpp"
#include "bpmodule/modulemanager/ModuleManager.hpp"

namespace bpmodule {
namespace modulebase {

    typedef std::vector<system::Atom> AtomV_t;
    typedef modulemanager::ModuleManager MM_t;
class FDFunctor{
    private:
        size_t Order_;
        const AtomV_t& Atoms_;
        MM_t& MM_;
        std::string Key_;
        unsigned long ID_;
    public:
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
            modulemanager::ModulePtr<Method> NewMode=
                    MM_.GetModule<Method>(Key_,ID_);
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
    
 
std::vector<double> Method::Deriv(size_t Order){
    //if(Order==0)//Throw error
    const system::Molecule& Mol=*Wfn().system;
    //std::vector<system::Atom> Atoms(Mol.begin(),Mol.end());
    const LibTaskForce::Communicator& Comm=*Wfn().comm;
    std::cout<<Comm<<std::endl;
    std::cout<<Mol<<std::endl;
    
    //math::CentralDiff<double,double> FD(Comm.Split());
    //FD.Run(FDFunctor(Order,Atoms,MManager(),ID(),Key()));
    return std::vector<double>(2);
}
    
}}
