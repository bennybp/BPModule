Making and Using Basis Sets in Pulsar                                {#basisset}
=====================================

Arguably the most fundamental set of data required for electronic structure
theory is the basis set.  These are the functions that we use to build up the
wave descriptions of each electron and thus are in a certain light the electrons
themselves.  Keeping with the Pulsar philosophy, we have tried to make this
class as genaric as possible while still maintaining usability.

## Types of Basis Sets
In electronic structure theory there are two main types of basis sets that are
commonly encountered"
- Atom-centered basis sets
  - Gaussian or Slater type orbitals, radial grids
- Atom-agnostic basis sets
  - Plane waves, rectangular grids
We strive to be able to handle both types of basis sets within Pulsar although
at the moment we have only considered shell-like basis sets.

## Atom-Centered Basis Sets

### Background

The use of Gaussian functions is usually motivated by first considering Slater 
functions:
\f[
s(\vec{R};\alpha,\vec{\ell})=N(\alpha,\vec{\ell})\vec{R}^\vec{\ell}e^{-\alpha\vec{R}}
\f]
where \f$\vec{R}\f$ is a vector from the origin to the center of the basis 
function, \f$\alpha\f$ is the exponent, \f$\ell\f$ is a quantity that will be
associated with the orbital angular momentum, and \f$N\f$ is a normalization 
constant.  Gaussian basis functions are then obtained by squaring \f$\vec{R}\f$
in the exponential:
\f[
g(\vec{R};\alpha,\vector{\ell})=N(\alpha,\vect{\ell})\vec{R}^\vec{\ell}e^{-\alpha R^2},
\f]
The result is that the exponential is no longer vector-like, but scalar and
consequentially the product fo multiple Gaussians is easier than that of the
Slater functions (although the actual form is still horrendous).  

What we have just described are so called primitives.  Slater functions have 
the right physical form, but Gaussian functions do not.  To get around this
one typically defines a contracted Gaussian,\f$G(\vec{R};\vec{c},\alpha,\vector{\ell})\f$
\f[
G(\vec{R};\vec{c},\vec{\alpha},\vector{\ell})=
    \sum_i c_ig(\vec{R};\alpha_i,\vector{\ell})
\f]
which is fit to recover the physical properties of a particular atom.  It is
linear combinations of the contracted Gaussians that are used to form the
molecular orbitals of the molecule.  The set of all contracted Gaussians for a
particular value of \f$\vec{\ell}\f$ is termed a shell and is the lowest level
of use to most users.

### Basis Shell Classes

As mentioned shells are the most basic source of information about the basis set
they are stored in the BasisShellInfo class, which can be used like:
~~~{.cpp}
BasisShellInfo AShell;//Assume this was initialized

ShellType AShell.get_type();//Returns Cartesian, spherical, or Slater

int L=AShell.am();//Returns the angular momentum of the shell

int NPrim=AShell.n_primitives();//Returns the number of primitives in the shell

//Returns the number of basis functions accounting for spherical/Cartesian
int NFunc=AShell.n_functions();

//Returns the alpha value for the i-th primitive
double alpha=AShell.get_alpha(i);

//Returns the coefficient of the i-th primitive
double c=AShell.get_coef(0,i);//Ignore what the 0 means for now
~~~

\note There is also a BasisSetShell class, which is designed for use in more
performance critical code, but runs the risk of being changed from under you
(it's sort of like an iterator, if the container under it changes, its garbage).
The API of the BasisSetShell class is nearly identical to that of the
BasisShellInfo class as far as a typical user is concerned.

\todo Expand on our general contraction features

### Basis Set Class

The actual BasiSet class is a container for holding a set of BasisShellInfo 
instances.  The main features are:

~~~{.cpp}
BasisSet BS;//Assume it is filled and set up elsewhere

int NShells=BS.n_shells();//The number of shells currently in the BS

int NFxn=BS.n_functions();//Sum of n_functions for each shell

int NPrim=BS.n_primitives();//The total number of primitives

int NCoef=BS.n_coefficients();//The total number of coefficients

BasisShellInfo Shelli=BS.shell_info(i);//Get the info of a particular shell
~~~

## Plane Waves and Grids

\todo Implement and document me


