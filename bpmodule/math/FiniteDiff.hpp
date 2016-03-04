#ifndef SRC_LIB_LIBCADGER_LIBALGORITHM_FINITEDIFF_H_
#define SRC_LIB_LIBCADGER_LIBALGORITHM_FINITEDIFF_H_

#include<vector>
#include "LibParallel.hpp" 

namespace bpmodule{
namespace math{

/** \brief Abstract base class for an object meeting the requirements of
 *  a visitor that provides the minimal functionality to be used with the
 *  finite difference algorithm
 * 
 *  \param[in] VarType The type of each variable.  Must:
 *    - Have an assignment operator (operator=)
 *    - Support addition via the plus operator
 *  \param[in] ResultType The type that goes into the final derivative. Must:
 *    - Have an assignment operator (operator=)
 */
template<typename VarType,typename ResultType=VarType>    
struct FDiffVisitor{    
    ///Returns variable i
    VarType operator(size_t i)const=0;
    
    ///Instructs you to run your function with the i-th variable set to NewVar
    ResultType operator(size_t i, VarType NewVar)const=0;
    
    ///Instructs you to scale Result by coef
    ResultType operator(ResultType& Result,double Coef)const=0;
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
 *
 *
 *
 */
 std::vector<double> Coefs(const std::vector<double>& Stencil,size_t Deriv=1);

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
 *  Note that if f is actually the gradient of some other function \f$g(X)\f$,
 *  then \f$f_X\f$ is actually the Hessian, etc. for higher order derivatives.
 *  This means one may obtain an arbitrary order derivative by nesting calls.
 *  This class actually computes:
 *  \f[
 *    f_xh=\Gamma_h[f]
 *  \f]
 *  so you will need to scale your resulting array by one over \f$h\f$ to
 *  recover the derivative.  This may seem odd, but it allows for finite
 *  differences to be performed in other coordinate systems beside for
 *  Cartesian, see below for more details.
 *
  * \todo update this section
  * 
 *  Eventually one wants to perform finite difference in other coordinates
 *  than Cartesians, in particular internal coordinates.  Internal coordinates
 *  are difficult to work with, but we can get a feel for how one would
 *  implement them by considering the simpler problem of using this class
 *  to perform a finite difference calculation in polar coordinates.
 *
 *  In polar coordinates we define a class (detailed description
 *  follows):
 *  \code
 *     class PolarCoordinate{
 *        private:
 *           double r_;
 *           double theta_;
 *        public:
 *           //Traditional Constructor
 *           PolarCoordinate(double r=0,double theta=0):
 *              r_(r),theta_(theta){}
 *           //Copy Constructor
 *           PolarCoordinate(const PolarCoordinate& other):
 *              r_(other.r_),theta_(other.theta_){}
 *           //Assignment operator, note check for self-assignment
 *           const PolarCoordinate& operator=(const PolarCoordinate& other){
 *               if(this==&other)return *this;
 *               this->r_=other.r_;
 *               this->theta_=other.theta_;
 *               return *this;
 *           }
 *           //Projected addition
 *           PolarCoordinate operator+(const PolarCoordinate& Other){
 *                PolarCoordinate Result(*this);
 *                if(this->r!=0.0)Result.r+=Other.r;
 *                if(this->theta!=0.0)Result.theta+=Other.theta;
 *                return Result;
 *           }
 *           //Scaling
 *           PolarCoordinate operator*(const double& c){
 *              PolarCoordinate Result(*this);
 *              Result.r*=c;
 *              Result.theta*=c;
 *              return Result;
 *           }
 *     };
 *  \endcode
 *  and let VarType=PolarCoordinate.  Say we have an \f$N\f$ point system
 *  and we want to compute the gradient.  The gradient will be a \f$2N\f$
 *  element long array (we have a derivative with respect to r and theta at
 *  each point).  Our input to this class is then an
 *  array of \f$2N\f$ PolarCoordinate objects, each being either the r or
 *  the theta component of one of the points (this is completely analogous
 *  to our Cartesian case where each of the \f$N\f$ points has 3 coordinates
 *  and so our input was a \f$3N\f$ element array).  Our perturbation will
 *  also be a PolarCoordinate object, except it will have both an r and theta
 *  component.
 *
 *  After some internal setup, this class will then start running tasks.
 *  Say the first coordinate is the r component of point 1.  Thus it's
 *  internal state looks something like:\f$\lbrace r_1,0.0\rbrace\f$. If we
 *  are doing central difference we first shift it forward by the r component
 *  in our perturbation.  The next calculation shifts it backward by the r
 *  component.  Both shifts occur via the addition operator, which is why
 *  we have defined it as projected addition and not component wise addition.
 *  We then move onto the next coordinate, which logically is likely to
 *  be the theta component of point 1, although it doesn't need to be.
 *  Assuming it is, we then shift forward and backwards by the theta component
 *  of the perturbation.  Again, the projected addition takes care of
 *  this.
 *
 *  The description above would be repeated for each component and
 *  each shift.  The end result would be the derivative with each component
 *  scaled by its perturbation.  Assuming your polar components were in the
 *  order r of point 1, theta of point 1, r of point 2, etc.  You would
 *  have to divide the even components of your vector by the r component of
 *  the perturbation and the odd components by theta (odd/even following from
 *  C++ starting at 0).  I suspect in most cases the derivative will
 *  just be an array of doubles and hence each component would not be able
 *  to discern which component it needs to be divided by.  I could enforce
 *  an ordering, or place other restrictions on ResultType, but I'd rather
 *  not and hence it is up to you to scale each component correctly as you
 *  presumably know what order you gave me the components in...
 *
 *
 *  To use this class to compute the gradient, using a shift of 0.02 a.u.
 *  (technically whatever units your coords are in, but they SHOULD be in
 *  a.u.), the code would be:
 *  \code
 *     double* Molecule=FxnReturnsCoordinates();
 *     size_t NAtoms=FxnReturnsNumberOfAtoms();
 *     CentralDiff<double,double> FD(Molecule,3*NAtoms,0.02,1);
 *     //This also works because we assume the size of ReturnType is 1
 *     CentralDiff<double> FD(Molecule,3*NAtoms,0.02);
 *
 *     //Finally get the gradient, and scale it by 1/h
 *     std::vector<double> Gradient=FD.Run(Energy);
 *
 *     for(double Gi : Gradient)Gi*=50; // Note: 1/0.02=50
 *  \endcode
 *  where Energy is a callable object that has the signature:
 *  \code
 *     double* Energy(double*,size_t);
 *  \endcode
 *  Arguably the most straightforward and C++ way to accomplish this
 *  is via a functor:
 *  \code
 *  class EnergyFunctor{
 *     public:
 *        double* operator()(double* Coords,size_t NAtoms){
 *           return FxnThatComputesEnergyOfGeometryAndReturnsIt(Coords,NAtoms);
 *        }
 *  };
 *  \endcode
 *  Unfortunately, operators can't be static so the code above needs to
 *  create an instance of an EnergyFunctor.
 *
 *
 */
template<typename VarType,typename ResultType=VarType>
class FiniteDiff{
   private:
      //The communicator in charge of this FDiff
      LibTaskForce::Communicator& Comm_;
   protected:
      ///Function to generate the coefs, minor tweaks for backwards and central
      virtual std::vector<double> GetCoefs(size_t NPoints)const{
        std::vector<double> x(NPoints);
        for(size_t i=0;i<NPoints;i++)x[i]=Shift(i);
        return Coefs(x);
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
      
      ///Communicator on which to run the tasks
      FiniteDiff(LibTaskForce::Communicator& Comm):Comm_(Comm){}
      
      /** \brief Computes the actual finite difference
       * 
       *   \param[in] Fxn2Run The visitor that we will be using to run the
       *                      finite difference.  See FDVisitor class for
       *                       requirements
       *    \param[in] H      The perturbation from the point
       *    \param[in] NPoint How many points are in the stencil
       */ 
      template<typename Fxn_t>
      std::vector<ResultType> Run(Fxn_t Fxn2Run,
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
      std::vector<double> GetCoefs(size_t NPoints)const{
          std::vector<double> temp=Base_t::GetCoefs(NPoints);
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
std::vector<ResultType> 
   FiniteDiff<VarType,ResultType>::Run(Fxn_t Fxn2Run,
                                        size_t NVars,
                                       const VarType& H,
                                       size_t NPoints){
    std::vector<double> Coefs=GetCoefs(NPoints);
    
    //Little wrapper to pass to task scheduler
    class FDWrapper{
        private:
            Fxn_t& Fxn_;
            double Coef_;
        public:
            FDWrapper(Fxn_t& Fxn,double Coef):Fxn_(Fxn):Coef_(Coef){}
            ResultType operator()(VarType Coord,size_t i){
                //Have it compute the fxn's value at the new point
                ResultType result=Fxn_(i,Coord);
                //Have it scale the result
                result=Fxn_(result,Coef_);
                return result;
            }
    };
    //Will store our derivatives
    LibTaskForce::TaskResults<ResultType> Deriv_;
    
    //Complete loop before assigning results
    for(size_t i=0;i<NVars;++i){//Loop over variables
        VarType Old=Fxn2Diff(i);
        for(size_t j=0;j<NCalcs(NPoints);++j){//Loop over calcs per variable
            VarType Coord=Old + H*Shift(j,NPoints);
            Deriv_.push_back(
               Comm_.AddTask(FDWrapper(Fxn2Run,Coefs[j]),Coord,i)
            );
        }
    }
    
    std::vector<ResultType> Result(NVars);
    //Will deref the futures now
    for(size_t i=0;i<NVars;++i)Result[i]=Deriv_[i];
    return Result;
}

}}



#endif /* SRC_LIB_LIBCADGER_LIBALGORITHM_FINITEDIFF_H_ */
