#ifndef PULSAR_GUARD_MATH__FINITEDIFF_HPP_
#define PULSAR_GUARD_MATH__FINITEDIFF_HPP_

#include<vector>
#include "LibTaskForce.hpp" 

namespace pulsar{
namespace math{

/** \brief Abstract base class for an object meeting the requirements of
 *  a visitor that provides the minimal functionality to be used with the
 *  finite difference algorithm.
 * 

 *  We need your help  doing five tasks:
 * 
 *  1. Give us the \f$i\f$-th of the \f$N\f$ variables
 *     - Handled by operator()(size_t i)const
 *  2. Update the coordinate we give you
 *     - operator()(const VarType&,const VarType&,double)const
 *  3. Compute a ResultType given the new coord for the i-th variable
 *     - operator()(size_t,const VarType&)
 *  4. Scale a ResultType by a double
 *     - operator()(ResultType&,double)
 *  5. Update the result
 *     - operator()(ResultType&,ResultType&,size_t,const VarType&)const
 * 
 *  You can use the predefined functions in this class if VarType supports
 *  addition by the plus operator, multiplication from the right by a double, 
 *  and division from the right by its type.  ResultType must contain iterators
 *  and an insert operator.  Basically we assume that ResultType is an
 *  std::vector<VarType> and VarType is a simple type like double.  It should be
 *  possible, via inheritance, to use more complicated types though.
 * 
 *  \param[in] VarType The type of each variable.
 *  \param[in] ResultType The type that goes into the final derivative.
 */
template<typename VarType,typename ResultType=VarType>    
struct FDiffVisitor{
    ///Makes sure the compiler doesn't complain
    virtual ~FDiffVisitor()=default;    
    ///Returns variable i
    virtual VarType operator()(size_t i)const=0;
    
    ///Return Old+H*Shift
    virtual VarType operator()(const VarType& Old,
                             const VarType& H,
                             double Shift)const{
        return Old+H*Shift;
    }
    
    ///Instructs you to run your function with the i-th variable set to NewVar
    virtual ResultType operator()(size_t i, const VarType& NewVar)const=0;
    
    ///Instructs you to scale Result by coef (Result has been initialized)
    void operator()(ResultType& Result,double Coef)const{
        for(auto & i :Result)i*=Coef;
    }
    
    ///Instructs you to set the i-th element of Result to Element/H
    ///(Result may not have been initialized)
    void operator()(ResultType& Result,
                  ResultType& Element,
                  size_t,
                  const VarType& H)const{
        if(Result.size()==0){
            for(size_t i=0;i<Element.size();++i)
                Result.push_back(Element[i]/H);
        }
        else{
            for(size_t i=0;i<Result.size();++i)
                Result[i]+=Element[i]/H;
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

 /** \brief Base class for computing a derivative by finite difference
 *
 *  Given a set of \f$N\f$ variables \f$X=\lbrace X_i\rbrace\f$, a function
 *  that depends on those variables, \f$f(X)\f$, and a spacing \f$h\f$ we
 *  may define several ways of computing the difference between two points.
 *  In general we label that difference \f$\Gamma_h[f](X)\f$.  The derivative
 *  of \f$f\f$ with respect to \f$X\f$, \f$f_X\f$,  i.e. the gradient of
 *  \f$f\f$, is then given by:
 *  \f[
 *    f_X=\frac{\Gamma_h[f](X)}{h}.
 *  \f]
  
 *  Eventually one wants to perform finite difference in other coordinates
 *  than Cartesians, in particular internal coordinates.  The visitor class
 *  puts you in control of each math step that involves your objects, thus it
 *  should be possible to use any objects you want by defining the visitor's
 *  fxns appropriately.
 */
template<typename VarType,typename ResultType=VarType>
class FiniteDiff{
   private:
      //The communicator in charge of this FDiff
      LibTaskForce::Communicator& Comm_;
      typedef FiniteDiff<VarType,ResultType> My_t;
   protected:
      ///Function to generate the coefs, minor tweaks for backwards and central
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
      ///Default destructor is fine
      virtual ~FiniteDiff()=default;
      ///No default initialization b/c we want the communicator
      FiniteDiff()=delete;
      ///Really don't see why you want to copy/move this, it's use as needed and
      ///comm's dont move/copy
      FiniteDiff(const My_t&)=delete;
      FiniteDiff(const My_t&&)=delete;
      My_t operator=(const My_t&)=delete;
      
      ///Communicator on which to run the tasks
      FiniteDiff(LibTaskForce::Communicator& Comm):Comm_(Comm){}
      
      /** \brief Computes the actual finite difference
       * 
       *   \param[in] Fxn2Run The visitor that we will be using to run the
       *                      finite difference.  See FDVisitor class for
       *                       requirements
       *    \param[in] NVars  The number of variables
       *    \param[in] H      The perturbation from the point
       *    \param[in] NPoint How many points are in the stencil
       *    \return your derivative
       */ 
      template<typename Fxn_t>
      std::vector<ResultType> Run(Fxn_t Fxn2Run,
                                  size_t NVars,
                                  const VarType& H,
                                  size_t NPoint=3);
};



 /** \brief The forward difference
  *
  * The forward difference of \f$f\f$ at the point \f$X\f$,
  * \f$\Delta_h[f](X)\f$, is defined as:
  *  \f[
  *    \Gamma_h[f](X)=\Delta_h[f](X)=f(X+h)-f(X).
  *  \f]
  *
  *
  */
template<typename VarType,typename ReturnType=VarType>
class ForwardDiff: public FiniteDiff<VarType,ReturnType>{
   private:
       ///Type of the base class
       typedef FiniteDiff<VarType,ReturnType> Base_t;
      ///Just casts to double 
      double Shift(size_t I,size_t)const{return static_cast<double>(I);}
   public:
       ForwardDiff(LibTaskForce::Communicator& Comm):Base_t(Comm){}
};

/** \brief The backward difference
 *
 * The backward difference of \f$f\f$ at the point \f$X\f$,
 * \f$\bigtriangledown_h[f](X)\f$, is defined as:
 *  \f[
 *    \Gamma_h[f](X)=\bigtriangledown_h[f](X)=f(X)-f(X-h).
 *  \f]
 *
 *  The coefficients of the backwards difference are just those of the
 *  forward difference multiplied by negative one (for odd powered
 *  derivatives).
 *
 *
 */
template<typename VarType,typename ReturnType=VarType>
class BackwardDiff: public FiniteDiff<VarType,ReturnType>{
   private:
       ///Type of the base class
      typedef FiniteDiff<VarType,ReturnType> Base_t;
      
      ///Negates the shift and casts to double
      double Shift(size_t I,size_t)const{return -1.0*static_cast<double>(I);}
      
      ///Negates the normal coeffs
      std::vector<double> get_coefs(size_t NPoints)const{
          std::vector<double> temp=Base_t::get_coefs(NPoints);
          for(size_t i=0;i<temp.size();++i)temp[i]*=-1.0;
          return temp;
      }
   public:
      BackwardDiff(LibTaskForce::Communicator& Comm):Base_t(Comm){}

};

/** \brief The central difference is the usual way of taking a 
 *         finite difference
 *
 * The central difference of \f$f\f$ at the point \f$X\f$,
 * \f$\delta_h[f](X)\f$, is defined as:
 *  \f[
 *    \Gamma_h[f](X)=\delta_h[f](X)=f(X+h)-f(X-h).
 *  \f]
 *  This is what is termed the three-point central difference (in theory there
 *  is also an evaluation at f(X), but it has a coefficient of 0).  Central
 *  differences using an arbitrary odd number of points are available (the
 *  points have to be symmetrically distributed around the expansion point,
 *  hence the odd requirement, which also implies that 3 points is the minimum).
 *  The more points you use, the more accurate your resulting derivative
 *  will be, but the more calculations that must be performed.
 *
 *
 */
template<typename VarType,typename ReturnType=VarType>
class CentralDiff:public FiniteDiff<VarType,ReturnType>{
   private:
     typedef FiniteDiff<VarType,ReturnType> Base_t;  
     size_t NCalcs(size_t NPoints)const{
         ///\todo add assert that NPoints%2==0
         return NPoints-1;
     }
      double Shift(size_t I,size_t NPoints)const{
         const size_t Half=NCalcs(NPoints)/2.0;
         double shift=static_cast<double>(I)-static_cast<double>(Half);
         if(I>=Half)shift+=1.0;
         return shift;
      }

   public:
       CentralDiff(LibTaskForce::Communicator& Comm):Base_t(Comm){}
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
        public:
            FDWrapper(Fxn_t& Fxn,double Coef):Fxn_(Fxn),Coef_(Coef){}
            ResultType operator()(VarType Coord,size_t i){
                //Have it compute the fxn's value at the new point
                ResultType result=Fxn_(i,Coord);
                //Have it scale the result
                Fxn_(result,Coef_);
                return result;
            }
    };
    //Will store our derivatives
    LibTaskForce::TaskResults<ResultType> Deriv_(Comm_);
    //Complete loop before assigning results
    for(size_t i=0;i<NVars;++i){//Loop over variables
        VarType Old=Fxn2Run(i);
        for(size_t j=0;j<NCalcs(NPoints);++j){//Loop over calcs per variable
            Deriv_.push_back(
               Comm_.AddTask(
                   FDWrapper(Fxn2Run,Coefs[j]),
                   Fxn2Run(Old,H,Shift(j,NPoints)),
                   i
               )
            );
        }
    }
    
    std::vector<ResultType> Result(NVars);
    Deriv_.Synch();
    //Will deref the futures now, in order we added them
    for(size_t i=0;i<NVars;++i)
        for(size_t j=0;j<NCalcs(NPoints);++j)
            Fxn2Run(Result[i],Deriv_[i*NCalcs(NPoints)+j],i,H);
    return Result;
}

}}//End namespaces
#endif
