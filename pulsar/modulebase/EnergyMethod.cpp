/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "pulsar/modulebase/EnergyMethod.hpp"
#include "pulsar/math/FiniteDiff.hpp"
#include "pulsar/datastore/Wavefunction.hpp"
#include "pulsar/system/System.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/modulemanager/ModulePtr.hpp"
#include "pulsar/modulemanager/ModuleManager.hpp"
#include "pulsar/parallel/Parallel.hpp"
#include <LibTaskForce/LibTaskForce.hpp>

using std::vector;

using pulsar::Atom;
using pulsar::System;
using pulsar::Wavefunction;
using pulsar::AtomSetUniverse;
using LibTaskForce::HybridComm;
using pulsar::GeneralException;

typedef vector<Atom> AtomV_t;
typedef pulsar::ModuleManager MM_t;
typedef vector<double> Return_t;


namespace pulsar{

class FDFunctor:public FDiffVisitor<double,Return_t>{
    private:
        typedef  FDiffVisitor<double,Return_t> Base_t;
        typedef ModulePtr<EnergyMethod> Module_t;
        size_t Order_;
        const AtomV_t& Atoms_;
        MM_t& MM_;
        const Wavefunction & Wfn_;
        std::string Key_;
        ID_t ID_;
    public:

        //Returns the i-th Cartesian coordinate of our molecule
        double coord(size_t i)const{return Atoms_[(i-i%3)/3][i%3];}

        Return_t run(size_t i,const double& newcoord)const{
            AtomSetUniverse NewU;
            for(size_t j=0;j<Atoms_.size();++j){
                if(j==(i-i%3)/3) { //does this coord index belong to this atom?
                    Atom atmp(Atoms_[j]);
                    atmp[i%3]=newcoord;
                    NewU.insert(atmp);
                }
                else
                    NewU.insert(Atoms_[j]);
            }
            Module_t NewModule=MM_.get_module<EnergyMethod>(Key_,ID_);
            Wavefunction NewWfn(Wfn_);
            NewWfn.system = std::make_shared<const System>(System(NewU,true));
            return NewModule->deriv(Order_-1,NewWfn).second;
        }

        FDFunctor(size_t Order,const AtomV_t& Atoms,
                  MM_t& MM,const Wavefunction & Wfn, std::string Key,ID_t ID):
            Order_(Order),Atoms_(Atoms),MM_(MM),Wfn_(Wfn),Key_(Key),ID_(ID){}
};


size_t EnergyMethod::max_deriv_()const{
    return options().get<size_t>("MAX_DERIV");
}


DerivReturnType
EnergyMethod::finite_difference_(size_t Order, const Wavefunction & Wfn){
    if(Order==0)
        throw GeneralException("I do not know how to obtain an energy via "
                               "finite difference.");
    const System& Mol=*(Wfn.system);
    vector<Atom> Atoms;
    vector<Return_t> TempDeriv;
    //I don't know why the fill constructor is not working...
    for(const Atom& AnAtom: Mol)Atoms.push_back(AnAtom);
    
    const HybridComm& Comm=get_env().comm();
    std::unique_ptr<HybridComm> NewComm=Comm.split(Comm.nprocs(),1);

    CentralDiff<double,Return_t> FD(std::move(*NewComm));

    FDFunctor Thing2Run=FDFunctor(Order,Atoms,module_manager(),Wfn,key(),id());
    TempDeriv=FD.Run(Thing2Run,3*Mol.size(),
                     options().get<double>("FDIFF_DISPLACEMENT"),
                     options().get<size_t>("FDIFF_STENCIL_SIZE"));
    //Flatten the array & abuse fact that TempDeriv[0] is the first comp
    for(size_t i=1;i<TempDeriv.size();++i)
       for(double j :  TempDeriv[i])
           TempDeriv[0].push_back(j);
    
    DerivReturnType CWfn=
            create_child<EnergyMethod>(key())->deriv(Order-1,Wfn);
    return {CWfn.first, TempDeriv[0]};
}

}
