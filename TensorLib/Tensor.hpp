#ifndef TENSOR_H_
#define TENSOR_H_

#include <array>
#include "TensorSettings.hpp"


/*! \brief The main tensor wrapper class
 *
 *  Terminology.  Tensor terms vary depending on whom you talk to.  I will
 *  refer to the number of indices as the "rank" of the tensor, i.e. a
 *  scalar is a rank 0 tensor, a vector is a rank 1, and a matrix is a rank
 *  2.  Each index has a maximum value, the set of these values will be
 *  called the shape of the tensor,
 *  i.e. a scalar has the empty set, \f$\lbrace\emptyset\rbrace\f$,
 *  as a shape, a vector of length \f$m\f$ has a shape:
 *  \f$\lbrace m\rbrace\f$, a matrix that is \f$m\f$ by \f$n\f$ has a shape:
 *  \f$\lbrace m,n\rbrace\f$.
 *
 *  The general philosophy of this class is that you should never have to
 *  get the raw-data pointer out, nor should you ever have to introduce logic
 *  into your codes like: "if tensor is non-zero".  This class is
 *  intelligent and is designed to recognize optimizations, such as
 *  multiplying by zero, all by itself.  Running with this example when you
 *  declare a tensor's shape initially, that tensor is a zero tensor.
 *  Internally, this is modeled with a null pointer to the data, hence
 *  multiplying the zero tensor by any other appropriately sized tensor,
 *  will return another zero tensor almost immediately, as it amounts to
 *  checking for a null pointer and then establishing the final zero
 *  tensor's shape (a few integer arithmetic operations).  We exploit
 *  this in designing our tensors.
 *
 *  \todo This doesn't actually work yet, need to figure out how to to do
 *  sparse tensors for TA and have to manually do it for CTF
 *
 *  Before filling your tensor you should establish its shape.  There
 *  are a few ways to do this.  At construction you may pass
 *  an std::array<size_t,Rank> to the tensor, where element \f$i\f$ is
 *  the length of the \f$i\f$-th index.  Alternatively, you may pass each
 *  length to the constructor like:
 *  \code
 *  //Builds a 10 by 10 by 10 tensor
 *  Tensor<double,3> A(10,10,10);
 *  \endcode
 *  The tensor may also be default constructed and then you may use
 *  either filling option at a later point by invoking the SetShape()
 *  member function.  SetShape() should not be invoked if the default
 *  constructor has been used.
 *
 *  For some applications it is convenient to be able to construct a
 *  larger tensor from smaller tensors.  For example consider the
 *  computation of the interaction energy of a water dimer.  If we
 *  run each water monomer computation we end with the density of
 *  water 1, W1, and the density of water 2, W2.  A great starting
 *  guess for the density of the dimer is
 *  \f[
 *  \left(\begin{array}{cc}
 *     W1 &  0\\
 *      0 & W2
 *  \end{array}\right)
 *  \f]
 *  This is known as the direct sum.  This is performed by the +=()
 *  operator.  Note:
 *  \code
 *  Tensor<double,3> A(10,10,10);
 *  Tensor<double,3> B(10,10,10);
 *
 *  //Normal "element-wise" tensor addition
 *  A["i,j,k"]+=B["i,j,k"];
 *  //Direct sum
 *  A+=B;
 *  \endcode
 *  The + operator is also defined for direct summation, if one wishes to
 *  preserve the input tensors.
 *
 *
 *
 *  You may default construct a tensor, but it will
 *  not be usable until you set it's shape.  By
 *  shape I mean the lengths of each rank.  For
 *  example an \f$n\f$ by \f$n\f$ square matrix
 *  has a shape of \f$\lbrace n,n\rbrace\f$ and
 *  a \f$n\f$ by \f$m\f$ by \f$k\f$ rank 3 tensor
 *  has a shape of \f$\lbrace n,m,k\rbrace\f$.
 *
 *
 *  \param T The type this tensor contains.  May
 *      be double, complex, or Tensor<T2>
 *
 *  \param Rank The rank of this tensor
 *
 *  The following is an implementation detail and should
 *  not concern you.  Basically it is how we abstract the
 *  details of the underlying tensor library away.
 *
 *  \param Impl_t Class that wraps basic implementation details
 */

template<typename T,size_t Rank,typename Impl_t=TImpl<T,Rank> >
class Tensor{
   private:
      ///For my sanity and internal use only
      typedef typename Impl_t::TensorBase_t TBase_t;
      typedef typename Impl_t::IndexedTensor_t ITensor_t;
      typedef Impl_t::iterator iterator;
      //typedef Impl_t::const_iterator const_iterator;
      typedef Tensor<T,Rank,Impl_t> My_t;
   public:
      ///Convenient typedef of the Shape object
      typedef std::array<size_t,Rank> Shape_t;

      ///Makes a tensor which requires a shape to work
      Tensor():ActualTensor_(nullptr){}

      ///Frees Impl_
      ~Tensor(){if(ActualTensor_!=nullptr)delete ActualTensor_;}

      ///Makes a tensor given a shape
      Tensor(const Shape_t& Shape):
         ActualTensor_(Impl_t::Make(Shape)),Shape_(Shape){}

      template<typename... Args>
      Tensor(Args... args):
         ActualTensor_(Impl_t::Make(Shape_t({args...}))),Shape_({args...}){}

      ///How you specify the contraction pattern.
      ///(Don't worry about the signature...)
      ITensor_t operator[](const char* string){
         if(ActualTensor_==nullptr)//Shape better be set now
            ActualTensor_=Impl_t::Make(Shape_);
         return Impl_t::DeRef(string,*ActualTensor_);
      }

      ///Fills this tensor with a value (technically only the local block)
      void Fill(const T& value){
         Impl_t::Fill(value,*ActualTensor_);
      }

      ///Sets the shape of the tensor
      ///@{
      void SetShape(const Shape_t& Shape){Shape_=Shape;}

      template<typename... Args>
      void SetShape(Args... args){Shape_={args...};}
      ///@}


      /*! \brief begin/end iterators for looping over the data
       *
       *  This iterator should really only be used to fill the tensor.
       *
       *  In general the data in the tensor will be stored in a pattern that
       *  is unbeknownst to you.  That is you won't know where it resides or
       *  even which elements are next to each other in RAM or on disk.  The
       *  point of this class is to serve as the liaison to you so that you
       *  can fill in the tensor.  When you are given an instance of this
       *  class you will use it like:
       *  \code
       *  //Make a 10 by 10 matrix
       *  Tensor<double,2> T(10,10);
       *
       *  //Get an iterator to it's elements
       *  T::iterator Itr=T.begin(),ItrEnd=T.end();
       *
       *  //Buffer to hold the current index
       *  std::array<size_t,2> Idx
       *
       *  //Loop over the blocks contained in the iterator
       *  while(Itr!=ItrEnd){
       *    Idx=Itr.Index();
            *Itr=value(Idx);//Fills in element i,j
       *    ++Itr;
       *  }
       *  \endcode
       *
       *  \todo Implement const_iterator
       */
      ///@{
      iterator begin(){return Impl_t::begin(*ActualTensor_);}
      //const_iterator begin()const{return Impl_t::begin(*ActualTensor_);}
      iterator end(){return Impl_t::end(*ActualTensor_);}
      //const_iterator end()const{return Impl_t::end(*ActualTensor_);}
      ///@}

      std::ostream& operator<<(std::ostream& os)const{
         return Impl_t::PrintOut(os,*ActualTensor_);
      }



   private:
      ///The actual tensor
      TBase_t* ActualTensor_;

      ///The shape of our current tensor
      Shape_t Shape_;
};

template<typename T,size_t Rank,typename Impl_t>
inline std::ostream& operator<<(std::ostream& os,
      const Tensor<T,Rank,Impl_t>& Ten){
   Ten<<os;
   return os;
}


#endif /* TENSOR_H_ */
