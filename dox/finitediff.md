Finite Difference With Pulsar                                           {#fdiff}
=============================

Derivatives of functions and methods are a common occurence throughout
electronic structure theory.  Most of the time the formulas are not simple and
thus coding the derivatives is error prone.  Alternatively, you may need a
derivative higher than what is coded up at the moment.  In both scenarios finite
difference is useful.  In the former it gives you the correct answer to compare
your analytic derivative to and in the latter it allows you to continue on with
the computation despite the lack of the derivative.  Hence having the ability
to do finite difference is a useful coding tool for implementing new methods.

Most finite difference routines I have seen only worry about finite differencing
the energy of a method, such as Hartree-Fock, and do not worry about finite
differencing other functions.  One needs those derivatives too.  The finite
difference routine in Pulsar is designed to be as generic as possible.  This
page walks you through how to use it.

## Theory Behind Finite Difference

\todo Expand this section more thoroughly

Given a set of \f$N\f$ variables \f$X=\lbrace X_i\rbrace\f$, a function
that depends on those variables, \f$f(X)\f$, and a displacement \f$h\f$ we
may define several ways of computing the difference between two points.
In general we label that difference \f$\Gamma_h[f](X)\f$.  The derivative
of \f$f\f$ with respect to \f$X\f$, \f$f_X\f$,  i.e. the gradient of
\f$f\f$, is then given by:
\f[
   f_X=\frac{\Gamma_h[f](X)}{h}.
\f]

There are three main choices for \f$\Gamma_h\f$, the foward difference,
\f$\Delta_h[f](X)\f$:
\f[
   \Gamma_h[f](X)=\Delta_h[f](X)=f(X+h)-f(X),
\f]
the backward difference, \f$\bigtriangledown_h[f](X)\f$:
\f[
   \Gamma_h[f](X)=\bigtriangledown_h[f](X)=f(X)-f(X-h),
\f]
and the central difference, \f$\delta_h[f](X)\f$:
 \f[
    \Gamma_h[f](X)=\delta_h[f](X)=f(X+h)-f(X-h).
 \f]
 With the exception of the central difference these are so called 2 point finite
 difference formulas because the function is evaluated at 2 points (also called
 the stencil size).  The central difference is actually a 3 point finite
 difference because in theory there is also an evaluation at f(X), but it has a
 coefficient of 0.  It turns out the more points in the finite difference the
 more terms of the Taylor series of the derivative of the function you recover
 and consequentially the higher the accuracy.  This is the usual justification
 for using the central difference, because it in theory has 3 points at the
 cost of 2 points you actually get a better result for the same effort.
 
## Features of the Pulsar Finite Difference Routines

As alluded to in the introduction Pulsar's finite difference routines are
designed for flexability and generality.  They should allow differentiating
with respect to any form of variable, and with any return type.  Furthermore,
Pulsar is capable of forward, backward, and central finite differences with
arbitrary stencil sizes.  By nesting Finite Difference calls you can compute
any order derivative, e.g. finite differencing a finite differenced gradient
gives you a finite difference Hessian.  Although it does not explicitly exploit
symmetry the visitor pattern allows you to manually exploit it.  For example if
you know that shifting the fourth variable by x returns the same result as
shifting the fifth variable by x, you can arbitrarilly decide that when you're
given the fifth variable you'll actually pass the fourth (assuming you are using
the cache your computation will just immediatly return the answer).

## The Finite Difference Visitor Class

In computer science, every mathematical function can be written as a function,
which we call `fxn`, that takes a set of input variables, each of which are of
type `VarType`, and our function returns a single output of type `ReturnType`.
Usually the function takes a set of doubles and returns a double, but we do
not restrict to this case.  Additionally, we can pass in and return custom
types, which when used creatively, allows us to take derivatives in coordinate
systems other than Cartesian.  Anyways, the point is we expect that you have
some function with the signature:
~~~{.cpp}
ReturnType fxn(VarType coord1, VarType coord2,...,VarType coordN);
~~~
where exactly how you take the N coordinates is irrelevant (you can take them
as N arguments, a vector of length N, a set of size N, etc.).

Within the actual finite difference algorithm there is only a few points that
depend on what the coords actually are and what the return type actually is.  In
fact there is no point in the algorithm that cares about what goes on inside the
function.  All the algorithm needs to know is the set of coordinates, how to
shift and scale a coordinate, how to call your function with the modified 
coordinates, how to scale the function return, and finally how to assemble the
returns.

In order to convey this information to the finite difference algorithm we have
choosen the visitor pattern.  This pattern has you construct an object with a
member for each task we need you to perform.  Then when the algorithm needs to
do that task, it calls the appropriate member function.  In the current case
we have defined a visitor class with the following five functions:
~~~{.cpp}
template<typename VarType,typename ReturnType>
class MyFiniteDiffVisitor{
public:
    //When this function is called you will return the i-th coordinate
    VarType coord(size_t i)const=0;
    
    //When this function is called return Old+H*Shift
    VarType shift(const VarType& Old,const VarType& H,double Shift)const=0;

    //Run the function we are f-diffing with the i-th variable set to NewVar
    ReturnType run(size_t i,const VarType& NewVar)const=0;

    //Scale the RV you are given by the constant
    void scale()(ResultType& RV,double constant)const=0;

    //RV+=Element/H given that RV will be the derivative w.r.t. to the i-th 
    //input coordinate (that last piece of info is irrelevant in most scenarios)
    void update(ResultType& RV,ResultType& Element,size_t i,VarType H)const=0;
};
~~~

The actual visitor class provides default implementations for some of these
functions which will work if:
- `VarType` supports:
  - Addition with the plus operator
  - Multiplication from the right by a double
  - Division from the right by a variable of type `VarType`
- `ResultType` supports:
  - begin/end iterators
  - insert operator
These choices assume your `VarType` is something simple like a double and your
`ResultType` is something like an `std::vector`.  If this isn't the case you'll
need to override all of the functions.

There is one serious gotcha with the visitor class in that `RV` that is passed
to `update` may have only been default constructed.  This means that if you are
returning something like `std::vector` you'll need to check that it has enough
elements allocated, and if it doesn't, allocate them.

## Actually Running a Finite Difference

Once you have a visitor class for your function, you're almost done.  You need
to decide which type of finite difference you are going to run: forward,
backward, or central, how many points you want in your stencil, and how far your
shifts are.  Then it's as simple as:
~~~{.cpp}
MyFDVisitorClass MyVisitor;//Make and setup an instance of your visitor

CentralDiff<VarType,ReturnType> FD;//Make an instance of your choosen FDiff

VarType ShiftSize;//How far we should shift our function for each displacement

size_t NPoints;//Is this a 3 or a 5 or a 7 or a...size stencil? 

std::vector<ReturnType> Deriv=FD.run(MyVisitor,NVars,ShiftSize,NPoints);
~~~
At the end of this code `Deriv` is an `NVars` long array that is your derivative
such that element \f$i\f$ is the derivative with respect to whatever coordinate
your visitor returned when asked for the \f$i\f$-th coordinate.


