/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "pulsar/modulebase/EnergyMethod.hpp"
#include "pulsar/math/FiniteDiff.hpp"
#include "pulsar/parallel/Parallel.hpp"

using Return_t=std::vector<double>;
namespace pulsar{

size_t EnergyMethod::max_deriv_()const{
    return options().get<size_t>("MAX_DERIV");
}

class FDFunctor:public FDiffVisitor<double,Return_t>{
private:
    size_t Order_;
    const std::vector<Atom>& Atoms_;
    const Wavefunction & Wfn_;
    ModulePtr<EnergyMethod> EMeth_;
public:
    double coord(size_t i)const{return Atoms_[(i-i%3)/3][i%3];}
    double shift(const double& Old,const double& H,double Shift)const{
        return Old+H*Shift;
    }
    Return_t run(size_t i,const double& newcoord)const{
        const Atom& AtomI=Atoms_[(i-i%3)/3];
        Atom AtomJ(AtomI);
        AtomJ[i%3]=newcoord;
        Wavefunction NewWfn(Wfn_);
        NewWfn.system=std::make_shared<const System>(std::move(
            Wfn_.system->transform(
                [&](const Atom& ai){
                    return ai!=AtomI?ai:AtomJ;
                }
            )
        ));
        auto temp=EMeth_->deriv(Order_-1,NewWfn).second;
        return temp;
    }
    
    void scale(Return_t& RV,double constant)const{
        for(double& i:RV)i*=constant;
    }

    //Set element i of RV to Element/H
    void update(Return_t& RV,Return_t& Element,size_t,double H)const{
        if(RV.size()<Element.size())RV.resize(Element.size(),0.0);
        for(size_t j=0;j<Element.size();j++)RV[j]+=Element[j]/H;
    }

    FDFunctor(size_t Order,const std::vector<Atom>& Atoms,
              const Wavefunction & Wfn,ModulePtr<EnergyMethod>&& EMeth):
        Order_(Order),Atoms_(Atoms),Wfn_(Wfn),EMeth_(std::move(EMeth)){}
};


DerivReturnType
EnergyMethod::finite_difference_(size_t Order, const Wavefunction & Wfn){
    if(Order==0)
        throw PulsarException("I do not know how to obtain an energy via "
                               "finite difference.");
    const System& Mol=*(Wfn.system);
    std::vector<Atom> Atoms(Mol.begin(),Mol.end());
    std::vector<Return_t> TempDeriv;
    //I don't know why the fill constructor is not working...
    //for(const Atom& AnAtom: Mol)Atoms.push_back(AnAtom);
    
    CentralDiff<double,Return_t> FD;
    auto EMeth=create_child<EnergyMethod>(key());

    TempDeriv=FD.Run(FDFunctor(Order,Atoms,Wfn,std::move(EMeth)),3*Mol.size(),
                     options().get<double>("FDIFF_DISPLACEMENT"),
                     options().get<size_t>("FDIFF_STENCIL_SIZE"));
    
    //Flatten the array & abuse fact that TempDeriv[0] is the first comp
    for(size_t i=1;i<TempDeriv.size();++i)
        TempDeriv[0].insert(TempDeriv[0].end(),TempDeriv[i].begin(),TempDeriv[i].end());
    
    
    return {Wfn, TempDeriv[0]};
}

}
