Tensors and Tensor Interfaces with Pulsar                             {#tensors}
=========================================

Electronic structure theory is naturally expressed in terms of tensors (or more
rigourously multi-dimensional arrays).  You can think of a tensor as a matrix
that has more than two indices.  As powerful as tensors are for formulating
quantum chemistry equations, actually coding them up is somewhat of a nightmare.
As you can imagine, the bookkeeping quickly becomes a nightmare and ensuring
you have the optimal way to perform an operation is tricky.

At the moment there are several powerful tensor libraries out there and we are
doing our best to accomodate all of them.  To this end the core of Pulsar
is tensor agnostic.  We only have need for matrices and use Eigen for that.
Nevertheless, modules will need to use tensors and effeciently passing those
tensors among modules remains a challange.  Presently we have taken a pragmatic
approach.  That can be best summarized as two fold: you are free to put whatever
you want in the cache and we encourage you to register your custom tensor types
with the framework.

This page aims to elucidate those two points in more detail.

## What Pulsar Provides

Before we worry about speed and all that.  Let's go over some basics.
In order to work with Pulsar, all tensors must be able to minimally do the
following:
- Return their size per dimension.  Handled by `sizes()` function
- Given a set of indices return an element.  Handled by `get_value()`
- Given a value and a set of indices set an element.  Handled by `set_value()`
Any other functionality is icing on the cake.  Pulsar implements these three
points as a class called `TensorImpl`.  Registering a custom tensor class
with Pulsar entails deriving from this class an adaptor class that implements
these three operations.

## How to Register a Tensor Class With Pulsar

Throughout the core, Pulsar uses Eigen.  Consequentially, an implementation of 
`TensorImpl` for Eigen exists.  Registering other tensor libraries should be
similar.  Here we show the main considerations required to register an Eigen
matrix and use it effectively with Pulsar.  This is a stripped down version of
the actual code that does this for Eigen.
~~~{.cpp}
///Implements a rank 2 tensor (i.e. matrix) of doubles with Eigen as the backend
class EigenMatrix : public TensorImpl<2,double> {
private:
    
    //Eigen::MatrixXd is an Eigen matrix of an arbitrary number of doubles
    Eigen::MatrixXd matrix_;

public:
        //Allows us to get the wrapped Eigen matrix back
        Eigen::MatrixXd get_matrix()const{
            return matrix_;
        }

        //The following three functions should be self explanatory
        virtual std::array<size_t, 2> sizes(void) const{
            return {matrix_.rows(),matrix_.cols()};
        }

        double get_value(const std::array<size_t,2>& idx)const{
            return matrix_(idx[0], idx[1]);
        }

        void set_value(const std::array<size_t,2>& idx,double val){
           matrix_(idx[0], idx[1])=val;
        }

};//End EigenMatrix
~~~
That's all it took to register an EigenMatrix (well, there needs to be some
constructors, but you get the idea).  Now Pulsar can store the Eigen matrix
and give it to other modules through the TensorImpl class.  

Here's where we get to talk about speed.  If I'm random module A and I get a
TensorImpl, I know that I could always do something like:
~~~{.cpp}
//This is the Pulsar Tensor I was given
TensorImpl<2,double>* SomeTensor=FxnThatReturnsATensor();
std::array<size_t,2> sizes=SomeTensor->sizes();
Eigen::MatrixXd MyMatrix(sizes[0],sizes[1]);
for(size_t i=0;i<sizes[0];i++)
   for(size_t j=0;j<sizes[1];j++)
      MyMatrix(i,j)=SomeTensor->get_value({i,j});
~~~
to get the Pulsar Tensor into my native tensor library.  This isn't the cheapest
thing in the world, but if the matrix is only something like the MO coefficents
it is not the end of the world in the grand scheme of say an MP2 or coupled
cluster computation.  The most expensive thing Pulsar passes in this manner is
the MO coefficients.  We can gain a lot of the speed back by embracing the idea
of supermodules though.

Within a supermodule modules are written using common libraries.  Theoretically
one of those common libraries will be a tensor library.  Furthermore, because
your modules are likely to be calling eachother we can cheat somewhat and try
to downcast the TensorImpl we get back, but before that an obligatory warning.

\warning Downcasting (casting away from the base class) although allowed by the 
C++ language is considered bad practice as it usually signals a poorly designed
interface.  Serialization/messaging is usually considered the exception and is
why we need to do it here.  If I just opened Pandora's box for you by revealing
its existence, please promptly forget it after you register your tensor class.

Anyways, we can now rewrite the code above, anticipating that the returned type
was likely generated by another module in our same supermodule and thus the base
class actually is an instance of a tensor from our library.
~~~{.cpp}
//Define a function that converts a Pulsar matrix into an Eigen matrix
Eigen::MatrixXd Pulsar2Eigen(TensorImpl<2,double>* SomeTensor){
    //Here's the downcast (seems harmless enough...)
    EigenMatrixImpl* EigenMatrxi=
        dynamic_cast<const EigenMatrixImpl>(SomeTensor);
    
    //The pointer is either null if we were wrong and TensorImpl isn't an
    //instance of EigenMatrixImpl or now points to the original EigenMatrixImpl
    //if we were right
    if(EigenMatrix)
      return EigenMatrix->get_matrix();
  
    //If we got here, it's not an EigenMatrixImpl and we better make one
    std::array<size_t,2> sizes=SomeTensor->sizes();
    Eigen::MatrixXd MyMatrix(sizes[0],sizes[1]);
    for(size_t i=0;i<sizes[0];i++)
        for(size_t j=0;j<sizes[1];j++)
            MyMatrix(i,j)=SomeTensor->get_value({i,j});
    return MyMatrix;
}
~~~

This function would be used like this:
~~~{.cpp}
//This is the Pulsar Tensor I was given
TensorImpl<2,double>* SomeTensor=FxnThatReturnsATensor();

//"cast" it to an Eigen::MatrixXd
Eigen::MatrixXd MyMatrix=Pulsar2Eigen(SomeTensor);
//Can now use all of Eigen's fun functions
~~~

A couple of points.
- If the cast succeeds this is really fast, basically a pointer dereference.
- If it succeeds the data is still complying with your tensor library's
  conventions (distributed, cyclic, or whatever)
- This is not restricted to double, but works with any type your tensor library
  supports
- If the cast fails you can't assume anything about the data other than you can
  element by element copy it
  - In particular the data may actually be distributed over 1,000s of nodes and
    thus the entire array may be very large
    - For a distributed tensor library who does the element-wise copy in
      parallel this isn't that bad (probably communication heavy unless you luck
      out and have the same layout)
    - For a local tensor library more care needs to be taken as you may not be
      able to get the entire tensor in RAM
- If the cast fails and you now have to replace the data in the original tensor
  you have a dilemma to face:
  - You can go through `set_value` and ensuring the data is still laid out in
    the original manner, making the resulting tensor compliant with whatever
    tensor library gave you the tensor 
  - You can swap the TensorImpl instance with your instance, making the tensor
    now compliant with your tensor library
  - Whether the first or second choice is better depends on the runtime setup
- Because Pulsar uses Eigen it is probably worth trying a cast to
  an Eigen matrix in your code in case it was generated by Pulsar.  Eigen
  allows you to get the raw data pointer using `data()`, be warned that Eigen
  is by default column major

  \todo Do we want to set the Eigen flag to row major?

## Using Pulsar's TensorImpl From Python

It's not the prettiest thing in the world, but Pulsar can export its TensorImpl,
matrix, and vector classes to Python.  To do this we rely on Pybind11's ability
to take Eigen matrices and vectors and convert them to numpy objects.  To the
user this all works fairly transparently:

~~~{.py}
import numpy as np

#Make a standard numpy array
mat=np.ndarray([[1,2,3],[4,5,6],[7,8,9]])

#Put it in an EigenMatrixImpl (for consistency we didn't change the name Py
#side, but you can think of this as NumpyMatrixImpl, if you want)
#At the moment this actually copies (has to do with some weirdness in Pybind11)
pulsar_mat=psr.math.EigenMatrixImpl(mat)

#Returns 5
pulsar_mat.get_value([2,2])

#Sets element 2,2 to 12
pulsar_mat.set_value([2,2],12)

#Gets the contents of the pulsar matrix as a numpy array
mat2=pulsar_mat.get_matrix()
~~~

## Blocked Tensors

Most of the tensors we will need to pass around among modules are related to
quantities that have symmetry and spin components.  To this end Pulsar provides
the `BlockByIrrepSpin` class.  This allows you to assign to each irrep,spin
pair a tensor.
\note This tensor is not contigious across spin or irrep dimensions if you need
that sort of functionality look elsewhere

## List of Tensors in Pulsar Core

Admittidly there is somewhat of a lot of indirection going on in the various
tensor classes.  To this end we have provided the following cheat sheet of the
types available and what they do.  Most of these are typedefs, but some are
actually classes.  Which is which is somewhat irrelevant as long as you know
what interface they have

- Satisfy TensorImpl interface
  - MatrixDImpl : A TensorImpl<2,double>
  - VectorDImpl : A TensorImpl<1,double>
- Satisfy TensorImpl interface and have accessors to Eigen matrices
  - EigenMatrixImpl : A MatrixDImpl for an Eigen matrix of doubles
  - EigenVectorImpl : A VectorDImpl for an Eigen vector of doubles
- Satisfy BlockByIrrepSpin interface
  - IrrepSpinMatrixD : An irrep by spin array of MatrixDImpl
  - IrrepSpinVectorD : An irrep by spin array of VectorDImpl
  - BlockedEigenMatrix : An irrep by spin array of EigenMatrixImpl
  - BlockedEigenVector : An irrep by spin array of EigenVectorImpl



