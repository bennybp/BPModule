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
s(\vec{R},\theta,\phi;\alpha,n,\ell,m)=N\vec{R}^{n-1}
      e^{-\alpha\vec{R}}Y_{m}^{\ell}(\theta,\phi)
\f]
where \f$\vec{R}\f$ is a vector from the origin to the center of the basis 
function, \f$\alpha\f$ is the exponent, \f$n\f$ is the principal quantum number,
\f$\ell\f$ is the orbital angular momentum quantum number, \f$m\f$ is the magnetic
quantum number, and \f$N\f$ is a normalization constant.  Gaussian basis 
functions are on the otherhand:
\f[
g(\vec{R};\alpha,\vec{\ell})=Nx^{\ell_x}y^{\ell_y}z^{\ell_z}
     e^{-\alpha R^2},
\f]
There are three main differences, first there is no longer a dependence on the
principal quantum number, second the angular momentum is accounted for using a
Cartesian description and not the spherical harmonics, and third the exponential
term is no longer vector-like, but scalar.  The net result is that products of 
multiple Gaussians are easier to work with than those of Slater functions (
although the actual form is still horrendous).

Unfortunately when you compare the functional form of a Slater to a Gaussian,
only the Slater has the correct physical form (cusp at the nucleus, radial nodes
depending on the principal quantum number, and exponential decay).  Gaussian 
functions lack all of these.  However, we can take a
linear combination of Gaussian functions to reproduce these properties.  When we
do this the Gaussian given by \f$g\f$ is termed a primitive Gaussian and the
resulting, more physical, function \f$G\f$, is termed a contracted Gaussian and
has the form:
\f[
G(\vec{R};\vec{c},\vec{\alpha},\vec{\ell})=
    \sum_i c_ig(\vec{R};\alpha_i,\vec{\ell})
\f]
Even if we have to consider linear combinations of many primitive Gaussians the
resulting integrals are far easier to evaluate so that computationally the
choice to use Gaussian functions is still a win.  Finally, note that the set of 
all contracted Gaussians for a particular value of \f$\vec{\ell}\f$ is termed a
shell.

### Segmented vs. General Basis Sets

There are two ways of creating contracted Gaussians.  The first and more common
way leads to a segmented basis set.  In this scheme each primitive is used in
one and only one contraction.  Assume \f$\lbrace g_1,g_2,g_3,g_4\rbrace\f$ are 
four primitives,
in a segmented scheme one possible way of contracting these functions is:
\f[
   G_1=c_1 g_1+c_2 g_2
\f]
and
\f[
   G_2=c_3 g_3 + c_4 g_4
\f]
Each of the four functions appears only once on the other hand the so-called
general contraction scheme allows a primitive to be used more than once, for
example assume we only had two primitives 
\f$\lbrace g_1^\prime,g_2^\prime\rbrace\f$ then one possible way of forming two
contracted Gaussians is:
\f[
   G_1^\prime=c_1^1 g_1^\prime+c_2^1 g_2^\prime
\f]
and
\f[
   G_2^\prime=c_1^2 g_1^\prime + c_2^2 g_2^\prime
\f]

Most packages can not correctly support general contractions because they were
set up for segmented contractions and (optimally) supporting general contractions
requires rewriting the underlying integral routines.  Instead, note that 
defining \f$c_1=c_1^1\f$, \f$g_1=g_1^\prime\f$, \f$c_2=c_2^1\f$, 
\f$g_2=g_2^\prime\f$, \f$c_3=c_1^2\f$, \f$g_3=g_1^\prime\f$, \f$c_4=c_2^2\f$, 
and \f$g_4=g_2^\prime\f$ makes our general contraction look like a segmented
contraction; however the cost of the underlying integrals will be much greater
than it should be (essentially the same quantities will be computed more than
once because the code has no way of knowing they are the same).  Interestingly,
some of the older basis sets have quantities known as "sp" shells, which can
actually be viewed as a form of general contraction.

### Basis Set Class for Atom-Centered Basis Types

In a typical workflow you are either getting the basis set as input or setting
the basis set and returning it.  This section handles the former and the next
handles the latter.

For the most part when you deal with basis sets in Pulsar it will be through the
BasiSet class.  This class is obtainable from a System instance using the
`get_basis_set()` member function.  The resulting instance is the
main API we expect users to go through.  The main member functions are:

~~~{.cpp}
//Atoms can have multiple basis sets, so each one is tagged, this gets the
//one with the tag "PRIMARY" off all atoms
BasisSet BS=MyMol.get_basis_set("PRIMARY");//Assume MyMol was set up elsewhere

int NShells=BS.n_shells();//The number of shells currently in the BS

int NFxn=BS.n_functions();//Sum of n_functions for each shell

int NPrim=BS.n_primitives();//The total number of primitives

int NCoef=BS.n_coefficients();//The total number of coefficients

const BasisSetShell& Shelli=BS.shell(i);//Get the info of a particular shell

~~~

The BasisSetShell class is your main API to the data inside a shell.  The shell
obtained in the last line would be used like:

~~~{.cpp}
ShellType st=Shelli.get_type();//Returns Cartesian, spherical, or Slater

int L=Shelli.am();//Returns the angular momentum of the shell

int NPrim=Shelli.n_primitives();//Returns the number of primitives in the shell

//Returns the number of basis functions accounting for spherical/Cartesian
int NFunc=Shelli.n_functions();

//Returns the alpha value for the i-th primitive
double alpha=Shelli.get_alpha(i);

//Returns the coefficient of the i-th primitive of the 0-th general contraction
double c=Shelli.get_coef(0,i);
~~~

As with most of the Pulsar Core Python interfaces are available.  The two code
blocks together look like:
~~~{.py}
#Atoms can have multiple basis sets, so each one is tagged, this gets the
#one with the tag "PRIMARY" off all atoms
BS=MyMol.get_basis_set("PRIMARY")#Assume MyMol was set up elsewhere

NShells=BS.n_shells()#The number of shells currently in the BS

NFxn=BS.n_functions()#Sum of n_functions for each shell

NPrim=BS.n_primitives()#The total number of primitives

NCoef=BS.n_coefficients()#The total number of coefficients

Shelli=BS.shell(i)#Get the info of a particular shell

ShellT=Shelli.get_type()#Returns Cartesian, spherical, or Slater

L=Shelli.am()#Returns the angular momentum of the shell

NPrim=Shelli.n_primitives()#Returns the number of primitives in the shell

#Returns the number of basis functions accounting for spherical/Cartesian
NFunc=Shelli.n_functions()

#Returns the alpha value for the i-th primitive
alpha=Shelli.get_alpha(i)

#Returns the coefficient of the i-th primitive of the 0-th general contraction
c=Shelli.get_coef(0,i)
~~~

\todo Expand on our general contraction features

### Adding a Basis Set to a System

Each atom stores a map from a tag to a set of shells associated with that tag.
The tag is meant to distinguish among the various types of basis sets an atom
may have (primary, density fit, etc.).  The shells themselves are stored in a
lightweight class called BasisInfo, which is basically nothing more than a
vector of shells and a string description.  Each Shell is an instance of the
BasisShellInfo class, which can be made and inserted into a particular atom
like:
~~~{.cpp}
Atom H;//Already made hydrogen atom

BasisShellInfo Shell1(CartesianGaussian, //or SphericalGaussian or Slater
               l, //The angular momentum 0,1,2,...
               n_prim, //The number of primitives
               n_gen, //The number of general contractions
               alpha, //The exponents
               coefs //An n_gen by n_prim arrray of coefficients
);

H.basis_sets["BasisDescription"].shells.push_back(Shell1);
~~~
That's all you have to do to add a basis set to an atom.  You would then follow
the instructions on [How To Make and Use Systems](@ref systems) to add the atoms
to the system (the final basis set object would then be generated for you by
calling the get_basis_set function of the System).

Of course you can add basis functions to atoms in Python as well:

~~~{.py}
H=Atom()#Already made hydrogen atom

Shell1=BasisShellInfo(CartesianGaussian, #or SphericalGaussian or Slater
               l, #The angular momentum 0,1,2,...
               n_prim, #The number of primitives
               n_gen, #The number of general contractions
               alpha, #The exponents
               coefs #An n_gen by n_prim arrray of coefficients
)

H.basis_sets={"BasisDescription" : [Shell1]}
~~~

\question I don't know if its a glitch in the Python bindings of the class or 
what, but it appears you can not simply append to the shells instance in the 
BasisInfo instance, but rather have to set all the shells at once

## Plane Waves and Grids

\todo Implement and document me


