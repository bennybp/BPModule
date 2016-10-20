#ifndef PULSAR_GUARD_MATH__FINITEDIFF_HPP_
#define PULSAR_GUARD_MATH__FINITEDIFF_HPP_

#include <vector>
#include <cstdlib>
#include <LibTaskForce/LibTaskForce.hpp>
#include "pulsar/exception/Exceptions.hpp"

namespace pulsar{
namespace math{

/** \brief Abstract base class for an object meeting the requirements of
 *  a visitor that provides the minimal functionality to be used with the
 *  finite difference algorithm.
 * 
 *  See [Finite Difference With Pulsar](@ref fdiff) for a more detailed
 *  description of how to use this class and the FDiff technology in general
 * 
 *  \param[in] VarType The type of each variable.
 *  \param[in] ResultType The type of the final derivative
 */
template<typename VarType,typename ResultType>    
struct FDiffVisitor{
    ///Makes sure the compiler doesn't complain
    virtual ~FDiffVisitor()=default;    
    ///Returns variable i
    virtual VarType coord(size_t i)const=0;
    
    ///Return Old+H*Shift
    virtual VarType shift(const VarType& Old,const VarType& H,double Shift)const
    {
        return Old+H*Shift;
    }
    
    ///Instructs you to run your function with the i-th variable set to NewVar
    virtual ResultType run(size_t i, const VarType& NewVar)const=0;
    
    ///Instructs you to scale Result by coef (Result has been initialized)
    void scale(ResultType& Result,double Coef)const
    {
        for(auto & i :Result)i*=Coef;
    }
    
    ///Instructs you to sum into the i-th element of Result the quantity
    ///Element/H (Result may not have been initialized)
    void update(ResultType& Result,ResultType& Element,
                  size_t,const VarType& H)const
    {
        if(Result.size()==0){
            for(size_t i=0;i<Element.size();++i)
                Result.insert(Result.end(),Element[i]/H);
        }
        else{
            for(size_t i=0;i<Result.size();++i)
                Result[i]=Result[i]+Element[i]/H;//Avoid needing += operator
        }
    }
};    
    
    
/** \brief Function that generates the finite difference coefficients
 *         given the stencil
 *
 *  Full disclosure this algorithm comes from:
 *  https://amath.colorado.edu/faculty/fornberg/Docs/sirev_cl.pdf
 *
 *  Given a stencil (the points at which the finite difference is to
 *  be evaluated) and the derivative in question, this function returns
 *  the coefficients in the order the points where given.  Note you
 *  may not pass a stencil value of 0 in.
 *
 *  For example to use with a three-point central difference your stencil
 *  is: \f$\lbrace -1,1\rbrace\f$.  A five-point central difference is:
 *  \f$\lbrace -2,-1,1,2\rbrace\f$.  The output will be:
 *  \f$\lbrace 0.5,-0.5\f$ and
 *  \f$\lbrace 0.08333,-0.666666,0.66666,-0.08333\rbrace\f$ respectively.
 *  Note that these coefficients include the denominator already.
 */
 std::vector<double> coefs(const std::vector<double>& Stencil,size_t Deriv=1);

 ///Base class for computing a derivative by FDiff
template<typename VarType,typename ResultType>
class FiniteDiff{
   private:
      std::unique_ptr<LibTaskForce::HybridComm> Comm_;///<Comm for parallel
      typedef FiniteDiff<VarType,ResultType> My_t;//The type of this class
   protected:
      ///Function to generate the coefs, minor tweaks for central
      virtual std::vector<double> get_coefs(size_t NPoints)const{
        std::vector<double> x(NPoints);
        for(size_t i=0;i<NPoints;i++)x[i]=Shift(i,NPoints);
        return coefs(x);
      }

      ///Returns the number of calculations per variable when user requests
      ///an NPoints long stencil (tweaked by central)
      virtual size_t NCalcs(size_t NPoints)const{return NPoints;}

      ///Returns the shift for the i-th calculation of an NPoint FDiff
      virtual double Shift(size_t I,size_t NPoint)const=0;
      
   public:
      virtual ~FiniteDiff()=default;///<Default destructor, shuts compiler up
      FiniteDiff(const My_t&)=delete;///<One time use class
      FiniteDiff(const My_t&&)=delete;///<One time use class
      My_t& operator=(const My_t&)=delete;///<One time use class
      
      ///Makes a new FDiff that can run in parallel on Comm
      FiniteDiff(LibTaskForce::HybridComm&& Comm)
        :Comm_(new LibTaskForce::HybridComm(std::move(Comm))){}
      
      ///Normal FDiff, no parallel
      FiniteDiff(){}
      
      ///FDiff fxn \p Fxn2Run of \p NVars variables with \p NPoint displacements
      ///of size \p H
      template<typename Fxn_t>
      std::vector<ResultType> Run(Fxn_t Fxn2Run,size_t NVars,
                                  const VarType& H,size_t NPoint=3);
};

 ///Specializes shift for a Foward Diff
template<typename VarType,typename ReturnType>
class ForwardDiff: public FiniteDiff<VarType,ReturnType>{
   private:
       typedef FiniteDiff<VarType,ReturnType> Base_t;///<Type of the base class
      ///\copydoc FiniteDiff::Shift
      double Shift(size_t I,size_t)const{return static_cast<double>(I);}
   public:
       using FiniteDiff<VarType,ReturnType>::FiniteDiff;
};

///Specializes Shift for a Backward FDiff
template<typename VarType,typename ReturnType>
class BackwardDiff: public FiniteDiff<VarType,ReturnType>{
   private:
      typedef FiniteDiff<VarType,ReturnType> Base_t;///<Type of the base class
      ///\copydoc FiniteDiff::Shift
      double Shift(size_t I,size_t)const{return -1.0*static_cast<double>(I);}
      
   public:
       using FiniteDiff<VarType,ReturnType>::FiniteDiff;

};

///Specializes Shift and NCalcs for a central difference
template<typename VarType,typename ReturnType>
class CentralDiff:public FiniteDiff<VarType,ReturnType>{
   private:
     ///\copydoc FiniteDiff::NCalcs
     size_t NCalcs(size_t NPoints)const{
         if(NPoints%2!=1||NPoints==1)
             throw GeneralException("Central difference requires an odd number"
                                    " of points > 1");
         return NPoints-1;
     }
     
     ///\copydoc FiniteDiff::Shift
      double Shift(size_t I,size_t NPoints)const{
         const size_t Half=NCalcs(NPoints)/2;
         double shift=static_cast<double>(I)-static_cast<double>(Half);
         if(I>=Half)shift+=1.0;
         return shift;
      }
   public:
       using FiniteDiff<VarType,ReturnType>::FiniteDiff;
};



/************Implementations***************/
template<typename VarType,typename ResultType>
template<typename Fxn_t>
std::vector<ResultType> FiniteDiff<VarType,ResultType>::Run(Fxn_t Fxn2Run,
                                        size_t NVars,
                                       const VarType& H,
                                       size_t NPoints){
    std::vector<double> Coefs=get_coefs(NPoints);
    
    //Little wrapper to pass to task scheduler
    class FDWrapper{
        private:
            Fxn_t& Fxn_;
            double Coef_;
            VarType Coord_;
            size_t i_;
        public:
            FDWrapper(Fxn_t& Fxn,double Coef,VarType Coord, size_t i):
               Fxn_(Fxn),Coef_(Coef),Coord_(Coord),i_(i){}
            ResultType operator()()const{
                ResultType result=Fxn_.run(i_,Coord_);
                Fxn_.scale(result,Coef_);
                return result;
            }
            //Wrapper for TaskForce
            ResultType operator()(const LibTaskForce::HybridComm&)const{
                return (*this)();
            }
    };
    //Will store our derivatives
    std::vector<LibTaskForce::HybridFuture<ResultType>> Derivs;
    
    //This is for parallel, note need to complete loop before assigning results
    for(size_t i=0;i<NVars && Comm_;++i){
        VarType Old=Fxn2Run.coord(i);
        for(size_t j=0;j<NCalcs(NPoints);++j){
            const double da_shift=Shift(j,NPoints);
            FDWrapper wrap(Fxn2Run,Coefs[j],Fxn2Run.shift(Old,H,da_shift),i);
            Derivs.push_back(Comm_->add_task<ResultType>(wrap));
        }
    }
    
    std::vector<ResultType> Result(NVars);
    for(size_t i=0;i<NVars;++i){
        VarType Old=Fxn2Run.coord(i);
        for(size_t j=0;j<NCalcs(NPoints);++j){
            const double da_shift=Shift(j,NPoints);
            FDWrapper wrap(Fxn2Run,Coefs[j],Fxn2Run.shift(Old,H,da_shift),i);
            ResultType DerivI=(Comm_?
                Derivs[i*NCalcs(NPoints)+j].get():wrap());
            Fxn2Run.update(Result[i],DerivI,i,H);
        }
    }
    return Result;
}

}}//End namespaces
#endif
