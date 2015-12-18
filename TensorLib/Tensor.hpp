#ifndef TENSOR_H_
#define TENSOR_H_

#include <array>
#include "TensorSettings.hpp"
#include "Shape.hpp"
#include "View.hpp"

namespace TensorWrap {

/*! \brief The main tensor wrapper class
 *
 *  Terminology.  Tensor terms vary depending on whom you talk to.  I will
 *  refer to the number of indices as the "rank" of the tensor, i.e. a
 *  scalar is a rank 0 tensor, a vector is a rank 1, and a matrix is a rank
 *  2 tensor.  Each index has a maximum value, the set of these values will be
 *  called the shape of the tensor,
 *  i.e. a scalar has the empty set, \f$\lbrace\emptyset\rbrace\f$,
 *  as a shape, a vector of length \f$m\f$ has a shape:
 *  \f$\lbrace m\rbrace\f$, a matrix that is \f$m\f$ by \f$n\f$ has a shape:
 *  \f$\lbrace m,n\rbrace\f$.
 *
 *  Consider an \f$m\f$ by \f$n\f$ matrix, for which we want element
 *  \f$i,j\f$.  In C++ this is commonly done via an offset from the start
 *  of the array, e.g. :
 *  \code
 *  std::vector<double> Matrix(m*n);
 *  double E_IJ=Matrix[i*n+j];
 *  \endcode
 *  We refer to this as offset notation, that is we will say the above
 *  like: "give me the element at offset i*n+j".  The less computer
 *  programmy way of specifying an element, i.e. "give me element i,j"
 *  will be referred to as coordinate notation.  Note that for a
 *  scalar and a vector coordinate and offset notation are identical.  Also
 *  note that offset notation requires you to know how the data is laid
 *  out, specifically the above code is for so-called row major (the matrix
 *  is stored as rows, alternatively we say the column runs fast because
 *  looping over the data sequentially changes the column more than the
 *  row).  If the data was stored in column major form (that is the
 *  row runs fast) we instead have:
 *  \code
 *  std::vector<double> Matrix(m*n);
 *  //Pretending C++ is column major...(it's not)
 *  double E_IJ=Matrix[i+j*m]
 *  \endcode
 *  As you can see this means you need to know how the data is laid out in
 *  memory, which quite frankly, if you are using this class you don't know
 *  how it is laid out as it may vary...
 *
 *  The reality is the only time you should be accessing
 *  a tensor element-wise is when you are filling it.  The offset notation
 *  is good for initializing a tensor with a single value, but we
 *  provide the "Fill" fxn for that purpose (tensors default
 *  to zero).  For more complicated fill patterns use coordinate notation,
 *  which is accessed via this class's iterator, here's an example that
 *  initializes each element to the sum of its coordinate indices:
 *  \code
 *  //10 by 10 matrix of doubles
 *  Tensor<2> A(10,10);
 *
 *  //Get A's iterators
 *  Tensor<2>::iterator Aij=A.begin(),AijEnd=A.end();
 *
 *  //Loop over the elements and fill Aij with the sum of its
 *  //coordinate indices
 *  for(;Aij!=AijEnd;++Aij){
 *     //Aij.Index()[0] is i and Aij.Index()[1] is j
 *     *Aij=Aij.Index()[0]+Aij.Index()[1];
 *  }
 *  \endcode
 *
 *  The general philosophy of this class is that you should never have to
 *  get the raw-data pointer out (nor do we let you) nor should you ever have
 *  to introduce logic into your codes like: "if tensor is non-zero".
 *  This class is
 *  intelligent and is designed to recognize optimizations, such as
 *  multiplying by zero, all by itself.  Running with this example when you
 *  declare a tensor's shape initially, that tensor is a zero tensor.
 *  This is handled with meta data, not by actually filling the tensor with
 *  zeros hence multiplying the zero tensor by any other appropriately sized tensor,
 *  will return another zero tensor almost immediately, as it amounts to
 *  a few metadata operations.  We exploit this in many places.
 *
 *  \todo It's unclear to me at this moment whether this actually is the case
 *
 *
 *  Before filling your tensor you should establish its shape.  There
 *  are a few ways to do this and I recommend you consult the SetShape()
 *  function's documentation for the full lowdown.
 *
 *  Let's now talk about how to build a labeled tensor.
 *  For sake of argument
 *  assume we are building the MO coefficients, which we presently take to be
 *  a rank 2 tensor that is number of atomic spin orbitals,  by number of
 *  molecular spin orbitals.  Furthermore assume that we have sorted both
 *  types of spin orbitals so that the alpha orbitals come before the
 *  beta orbitals.  Finally, assume that the molecular spin orbitals are
 *  then further subdivided into occupied and virtual.  Our final tensor
 *  looks like this:
 *  \f[
 *    C=\left(\begin{array}{cccc}
 *     C_{oa} &  C_{va}& 0 &0\\
 *     0 &  0 & C_{ob} &C_{vb}
 *  \end{array}\right)
 *  \f]
 *
 *  We can declare C like:
 *  \code
 *  //Need to specify where the block boundaries are along each rank
 *  tensor<double,2> C({{nbf,nbf},{na,nva,nb,nvb}});
 *  C.SetView(0,{"a","b"});
 *  C.SetView(1,{"a","b","o","v"},{2,2});
 *
 *  //alpha density in AO basis
 *  AORho_a["i,j"]=C("a","a","o")["i,k"]*C("a","a","o")["j,k"];
 *
 *  //total density in AO basis
 *  AORho["i,j"]=C("","","o")["i,k"]*C("","","o")["j,k"];
 *  \endcode
 *
 *  In this example, the first index is the spin of the AO, the second
 *  is the spin of the MO, the third is the MO occupation, the fourth is
 *  the particular spin AO, and the fifth is the particular spin MO of
 *  the given occupation.  Again, under the hood the zero contractions
 *  will not actually be done so laying our tensor out in this format
 *  incurs only a very small overhead amounting to checking if a block is
 *  zero before contracting.  If for some reason the atomic orbitals had
 *  different forms for alpha and beta this layout would allow us to take
 *  that into consideration without modifying the code.
 *
 *  In the above code we introduced two partitions along the first dimension,
 *  and four along the second; this leads to 8 blocks.  For the second
 *  dimension we laid our blocks out so that the occupation ran faster
 *  than the spin, this means we can think of our blocks in the second
 *  dimension as being a two by two matrix with spin on the rows and
 *  occupation on the columns.  Before you can do any permutations or
 *  manipulations using the view, you need to tell the class
 *  where the blocks started.
 *
 *
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
 *  This is known as the direct sum of W1 and W2.
 *  This is performed by the + operator.  Note:
 *  \code
 *  Tensor<double,3> A(10,10,10);
 *  Tensor<double,3> B(10,10,10);
 *
 *  //Normal "element-wise" tensor addition
 *  A["i,j,k"]+=B["i,j,k"];
 *  //Direct sum
 *  Tensor<double,3> C;
 *  C+=A;
 *  C+=B;
 *
 *  //Also ok:
 *  C=A+B;
 *
 *  //If we wanted to direct sum more than two tensors, we continue
 *  //summing using the += operator (C=A+B+A now)
 *  C+=A;
 *  \endcode
 *
 *  At this point we expand on the direct summation a bit.  Given
 *  two tensors, \f$A\f$ and \f$B\f$ with ranks \f$a\f$ and \f$b\f$
 *  respectively, the direct summation is only defined if \f$a=b\equiv R\f$.
 *  Letting the length of the \f$i\f$-th dimension of \f$A\f$ be \f$a_i\f$
 *  and that of \f$B\f$ be \f$b_i\f$.  The resulting tensor, \f$C\f$ is
 *  of the same rank and its \f$i\f$-th dimension is \f$c_i=a_i+b_i\f$.
 *  Letting \f$C(i_1,i_2,\ldots,i_R)\f$ be the element of \f$C\f$ with
 *  indicies \f$i_1,i_2,\ldots,i_R\f$, and similarly for \f$A\f$ and
 *  \f$B\f$, we have:
 *  \f[C(i_1,i_2,\ldots,i_R)=\left\lbrace
 *  \begin{array}{ll}
 *  A(i_1,i_2,\ldots,i_r) &i_n\le a_n\ \forall n\\
 *  B(i_1-a_1,i_2-a_2,\ldots,i_r-a_n) &0\le i_n-a_n \le b_n\ \forall n\\
 *  0 & otherwise
 *  \end{array}
 *  \right.
 *  \f]
 *  Schematically, this means \f$C\f$ looks like:
 *   *  \f[
 *  C=\left(\begin{array}{cc}
 *      A &  0\\
 *      0 & B
 *  \end{array}\right)
 *  \f]
 *
 *  In quantum chemistry, we usually want to build C, but be able to
 *  refer to its blocks still.  Internally, we accomplish this by thinking
 *  indices allow us to choose the block we are in, e.g. C(0,1) is the
 *  upper right zero matrix.
 *
 *
 *
 *  The direct sum combined with the fact that tensors default to zero
 *  tensors leads to a quick way to build a tensor, if you know the
 *  structure ahead of time.  We can build the above shape like:
 *  \code
 *  Tensor<double,2> W1(nbf,nbf),W2(nbf,nbf),W12;
 *  W12=W1+W2;  //All tensors are still 0, we just have a shape
 *  \endcode
 *  The resulting tensor, W12, can be thought of as
 *  rank four (you have two indices for specifying
 *  the monomer block and then two indices within each block
 *  for specifying the density element).  For convenience, we
 *  may want to refer to the first two indices based on which
 *  monomer they stem from, "W1" or "W2".  We can do this via:
 *  \code
 *  //Syntax is rank we are mapping to, then values, in the order we want
 *  W12.MapIndices(0,{"W1","W2"});
 *  W12.MapIndices(1,{"W1","W2"});
 *  \endcode
 *
 *  The W1 tensor can then be retrieved from W12 as:
 *  \code
 *  //Note the placement of the quotes and the use of operator()
 *  //vs. operator[]
 *  W1copy=W12("W1","W1");
 *
 *  //This would sum W1 and W2:
 *  W1pW2=W12["W1,W1"];
 * 
 *  //This is W1 squared
 *  W1_2["i,k"]=W12("W1","W1")["i,j"]*W12("W1","W1")["j,k"];
 *  \endcode
 * 
 *  Note that in C++ the default copy constructor, as
 *  well as the default assignment operator, are shallow
 *  copies (the value of all pointers, i.e. the address of the actual
 *  object, is copied, not the object).  We follow this convention for
 *  our tensor class as well. This is to say W1copy above is a shallow copy
 *  of the W1 block of W12, but W1_2 is a shallow copy of a
 *  "temporary" (quotes because no temporary is actually made,
 *  thanks to lazy evaluation, the temporary is just built in W1_2)
 *  and will thus represent a memory chunk distinct from W12.
 *
 *  You may default construct a tensor, but it will
 *  not be usable until you set it's shape (unless it's a scalar, then
 *  the shape is automatically set).
 *
 *  \param T The type this tensor contains.  May
 *      be double, complex, or Tensor<T2>
 *
 *  \param Rank The rank of this tensor
 *
 *  The following is an implementation detail and should
 *  not concern you.  If you are familiar with template meta-programming,
 *  this is a property map (see Boost's graph class or the Boost property
 *  map library for more details).  The idea is it wraps implementation
 *  details in a generic manner so that I don't have to worry about
 *  what is the syntax CTF uses vs. that of TA.
 *
 *  \param Impl_t Class that wraps basic implementation details, default
 *         is set in TensorSettings.hpp file based on which tensor backend
 *         the user has selected.
 */
template <size_t Rank, typename T=double, typename Impl_t=TImpl<T, Rank> >
class Tensor {
   private:
      ///For my sanity and internal use only
      typedef typename Impl_t::TensorBase_t TBase_t;
      typedef typename Impl_t::IndexedTensor_t ITensor_t;
      typedef Tensor<Rank, T, Impl_t> My_t;
      template <typename T>
      using IL=std::initializer_list<T>;
   public:

      ///The iterator types
      typedef typename Impl_t::iterator iterator;
      //typedef Impl_t::const_iterator const_iterator;

      ///Convenient typedef of the shape input object
      typedef std::array<size_t, Rank> Shape_t;

      ///Except for the default constructor, all constructors call SetShape
      ///so see its documentation for how to construct a tensor
      ///@{
      ///Makes a tensor which will not work until a shape is set
      Tensor()=default;

      ///Makes a tensor given the length of each rank in an std::array
      Tensor(const Shape_t& Shape){SetShape(Shape);}

      ///Makes a tensor given the length of each rank as arguments
      template <typename... Args>
      Tensor(Args... args){SetShape(args...);}


      /** \brief Makes a tensor with the partitions of the given lengths
       *
       *
       */
      Tensor(const IL<IL<size_t> >& Idx){SetShape(Idx);}
      ///@}
      ///All memory is under garbage collection
      ~Tensor()=default;



      /** How you specify the contraction pattern.
      * (Don't worry about the signature and don't try to save the
      * result to anything other than a tensor object...)
      */
      ITensor_t operator[](const char* string) {
         assert(ActualTensor_);
         return Impl_t::DeRef(string, *ActualTensor_);
      }

      /*! \brief Retrieves an element by coordinate index
       *
       *  \param[in] args strings that map to your blocks
       */
      ///@{
      template<typename...Args>
      My_t operator()(Args... args) {
         std::set<size_t> Blocks=View_(args...);
         My_t NewT(Shape_.SubShape(Blocks));
         NewT.ActualTensor_=std::shared_ptr<TBase_t>(
         Impl_t::Get(Blocks,Shape_,*ActualTensor_));
         return NewT;
      }
      ///@}

      ///Returns direct sum
      My_t& operator+(const My_t& Other)const{
         return (My_t(*this))+=Other;
      }

      ///Direct sum into this tensor, return *this
      My_t& operator+=(const My_t& Other) {
         //Now we have the right shape
         this->Shape_+=Other.Shape_;

         //Copy data (if it exists)
         //std::shared_ptr<TBase_t> Temp=ActualTensor_;
         //ActualTensor_=std::shared_ptr<TBase_t>(Impl_t::Make(Shape_));

         std::cout<<Shape_<<std::endl;
         return *this;
      }

      /** \brief Sets the shape of the tensor, destroys any shape that
       *   existed and updates view accordingly
       *
       *   These calls are provided as a convenience to you the user
       *   so that you can default construct a tensor and then
       *   set its shape later.  They really are only intended to
       *   be called once on a given Tensor object.  If you did
       *   not use the default constructor you shouldn't need to
       *   call these functions.
       *
       *   There are quite a few acceptable syntaxes for these functions
       *   provided to hopefully make the process of creating a tensor
       *   as easy as possible.  For all examples we assume that
       *   you have a rank \f$R\f$ tensor, where the \f$i\f$-th
       *   dimension is of length \f$r_i\f$.
       *
       *   1. You may simply present each \f$r_i\f$ value
       *   \code
       *   Tensor<3> T;
       *   T.SetShape(r1,r2,r3);
       *
       *   //Or in one step
       *   Tensor<3> T2(r1,r2,r3);
       *   \endcode
       *
       *   2. You may present an std::array of each value:
       *   \code
       *   Tensor<3> T;
       *
       *   //This is also accessible via Tensor<3>::Shape_t
       *   std::array<size_t,3> Shape({r1,r2,r3});
       *
       *   //i.e. this also works
       *   Tensor<3>::Shape_t Shape2({r1,r2,r3});
       *   T.SetShape(Shape);//or Shape2
       *
       *   //Or in one step
       *   Tensor<3> T2(Shape2);//or Shape
       *   \endcode
       *
       *   3. Finally, the most complicated form, which is best used when
       *   you want to create blocks within your tensor. In this case,
       *   you should have \f$R\f$ initializer lists within an outer
       *   initializer list.  Each of the inner initializer lists should
       *   contain the lengths of the partitions, in the order you
       *   want them e.g. for \f$R=2\f$ the form is:
       *   \code
       *   //Makes a 6 by 19 matrix composed of 6 blocks arranged 2 by 3
       *   //Block 1 is [0,2) by [0,3), Block 2 is [0,2) by [3,4)
       *   //Block 3 is [0,2) by [4,19),Block 4 is [2,4) by [0,3)
       *   //Block 5 is [2,4) by [3,4), Block 6 is [2,4) by [4,19)
       *   Tensor<2> A({{2,4},{3,1,15}});
       *   \endcode
       *
       *   The tensor built by this would look like:
       *   \f[
       *   A=\left(\begin{array}{ccc}
       *     1 &  2 & 3\\
       *     4 &  5 & 6
       *  \end{array}\right)
       *  \f]
       *
       *  If you want to label the blocks for your convenience see the
       *  SetView function.
       */
      ///@{
      ///Variadic wrapper, calls other SetShape
      template<typename...Args>
      void SetShape(Args...args){SetShape(Shape_t({args...}));}

      ///Sets the layout, conveys that info to the backend, and initializes
      ///the view
      template<typename T2>
      void SetShape(const T2& AShape){
         Shape_=Shape<Rank>(AShape);
         ActualTensor_.reset(Impl_t::Make(Shape_));
         View_=View<Rank>(Shape_);
      }
      ///@}

      ///Allows user mapping of dimension i
      ///@{
      ///Wrapper for an initializer list of names, but no layout.  Must name
      ///all partitions
      void SetView(size_t i,const IL<std::string>& Names){
         assert(Names.size()==(Shape_[i].size()-1));
         SetView(i,Names,{Names.size()});
      }

      ///Main SetView function
      void SetView(size_t i,const IL<std::string>& Names,
            const IL<size_t>& Layout){
         View_.ChangeView(i,std::vector<std::string>({Names}),
               std::vector<size_t>({Layout}));
      }
      ///@}


      ///Fills this tensor with a value (technically only the local block)
      void Fill(const T& value) {Impl_t::Fill(value,*ActualTensor_);}

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
      iterator begin() {return Impl_t::begin(*ActualTensor_);}
      //const_iterator begin()const{return Impl_t::begin(*ActualTensor_);}
      iterator end() {return Impl_t::end(*ActualTensor_);}
      //const_iterator end()const{return Impl_t::end(*ActualTensor_);}
      ///@}

      ///Prints this tensor to an ostream via the backend
      std::ostream& operator<<(std::ostream& os)const {
         return Impl_t::PrintOut(os,*ActualTensor_);
      }

      void Temp(){
         ActualTensor_->make_replicated();
      }

      private:
         ///The shape of our current tensor
         Shape<Rank> Shape_;

         ///The view of our current tensor
         View<Rank> View_;

         ///The actual tensor
         std::shared_ptr<TBase_t> ActualTensor_;

   };

///Specialization to a scalar, ignores backend
template <typename T, typename T2>
class Tensor<0, T, T2> {
   private:
      ///For my sanity and internal use only
      typedef std::array<T, 1> TensorBase_t;
   public:

      ///The iterator types
      typedef typename TensorBase_t::iterator iterator;
      //typedef Impl_t::const_iterator const_iterator;

      typedef std::array<size_t, 0> Shape_t;

      ///Makes a scalar, no need to SetShape
      Tensor()=default;

      ///No memory to free up
      ~Tensor()=default;

      ///Shape is ignored
      Tensor(const Shape_t&){}

      ///Shape is ignored
      template <typename ... Args>
      Tensor(Args...) {}

      ///How you specify the contraction pattern.
      ///(Don't worry about the signature...)
      T operator[](const char*) {return ActualTensor_[0];}

      ///How you retrieve an element, args are ignored
      template <typename ... Args>
      T operator()(Args...) const {return ActualTensor_[0];}

      ///Casts the current tensor to the type it is
      operator T() {return ActualTensor_[0];}

      ///Direct product (which is just scaling for a scalar)
      template <typename T2>
      T2 operator*(T2& Other) const {return (*this)(0)*Other;}

      ///Sets this scalar
      void Fill(const T& value) {ActualTensor_[0]=value;}

      ///Sets the shape of the scalar (does nothing)
      ///@{
      void SetShape(const Shape_t&) {}

      template <typename ... Args>
      void SetShape(Args...) {}
      ///@}

      ///@{
      iterator begin() {return ActualTensor_.begin();}

      //const_iterator begin()const{return Impl_t::begin(*ActualTensor_);}
      iterator end() {return ActualTensor_.end();}
      //const_iterator end()const{return Impl_t::end(*ActualTensor_);}
      ///@}

      ///Allows printing
      std::ostream& operator<<(std::ostream& os) const {
         return os<<ActualTensor_[0];
      }

   private:
      ///The actual tensor, array provides iterators
      std::array<T, 1> ActualTensor_;
};

}       //End namespace TensorWrap

template <size_t Rank, typename T, typename Impl_t>
inline std::ostream& operator<<(std::ostream& os,
      const TensorWrap::Tensor<Rank, T, Impl_t>& Ten) {
   Ten<<os;
   return os;
}

#endif /* TENSOR_H_ */
