System Fragmenters                                             {#system_fragger}
==================

Many methods in electronic structure theory rely on decomposing a system into a
series of subsystems.  Ususally these subsystems are termed fragments.  There is
an enormous number of possible ways to do this for an arbitrary system.  Hence
rather than restricting users to hard coded fragmentation schemes this module
base type allows users and develpers to implement their own algorithms for
fragmenting systems.  Then any algorithm that requires fragments simply asks
for a module key to a system fragmenter and uses the resulting module.

\note This also means a user can do a somewhat arbitrary assignment of atoms
to fragments via a lambda module.

Many of the methods that depend on fragments also depend on pairs of fragments
(i.e. dimers), triples of fragments (i.e. trimers), etc. to improve their
descriptions.  Although, these additional dimers, trimers are usually discussed
in the literature in a somewhat priveleged manner, they themselves are also
perfectly good fragments (albiet non-disjoint).  When viewed in this manner
they can be treated on the same grounds as the original fragments and we
collectively refer to fragments, dimers, etc. as subsystems.  Ultimately, 
regardless of how the subsystems are formed, to my knowledge, the final energy 
(or other extensive property) of the total system will be given as a linear 
combination of the energies (or properties) of the subsystems.

## Design Specifications

### Serial Number

Although the final equations of fragment based methods are expressible as linear
combinations, determining the coefficents of each subsystem is usually easier if
you know which fragments are present in each subsystem.  To that end we number
each of the original \f$N\f$ fragments, in the order they were formed.  We call
that number the fragment's serial number.  Next when dimers are formed by taking
the union of fragment \f$i\f$ and \f$j\f$ we label the dimer with the set
\f$\lbrace i,j\rbrace\f$, which is its serial number.  In general, the union of
\f$n\f$ fragments, such that \f$n\in[0,N)\f$, produces a set of \f$n\f$ numbers
that we in turn term the resulting subsystem's serial number.  

- All algorithms are responsible for assigning unique serial numbers to 
  fragments.  
- Serial numbers are arbitrary other than the serial number of the union of 
  \f$n\f$ fragments must be the union of the \f$n\f$ fragment's serial numbers.

### Weights

These are the coefficients of the linear combination.  They are always knowable
by the fragmentation algorithm because they have to do with the spatial symmetry
of the fragments and some manner of avoiding double counting (typically the
inclusion-exclusion principle).  Hence it is the job of the fragmenter to assign
these as well.  Assume that subsystem \f$i\f$ has property \f$\chi_i\f$, its
weight, \f$\chi_i\f$ should be defined such that:
\f[
   \chi\approx\sum_i c_i\chi_i,
\f]
where \f$\chi\f$ is the resulting approximate system property that results
without further consideration.  For simple methods like SAPT on two monomers 
\f$c_1=c_2=1\f$ (SAPT would further correct this \f$\chi\f$ with two-body
contributions).

### Recommended Option Names

For consistency, the recommended option key names for options related to the
SystemFragmenter class are:

- SYSTEM_FRAGMENTER_KEY : The key of a SystemFragmenter used to generate an
  initial set of fragments from which the final set of subsystems will be
  generated
- TRUNCATION_ORDER : Consider unions involving up to this number of fragments


\note Not all SystemFragmenter implementations will support all of these options
always check the documentation of the module you intend to use


## System Fragmenter API

The main API of the SystemFragmenter is:
~~~{.cpp}
ModulePtr<SystemFragmenter> MyFragger;//A SystemFragmenter we got from somewher
System AMol;//A molecule we got from somewhere

//Fragment the system, result is a map from serial number to NMerInfo
NMerSetType Frags=MyFragger->fragmentize(AMol);

//The number of subsystems
size_t NFrags=Frags.size()

//A typedef of an element in an NMerSetType
using value_type=std::map<SNType,NMerInfo>::value_type

for(value_type Elemi : Frags){
   SNType SNi=Elemi.first;//The serial number of the subsystem
   NMerInfo Fragi=Elemi.second//The NMerInfo struct
   SNType SNi_2=Fragi.SN;//The serial number is stored here too
   double ci=Fragi.weight;//The coefficient
   System Fi=Fragi.NMer;//The actual subsystem
}
~~~

and it is mirrored in Python:

~~~{.py}
MyFragger=SystemFragmenter()# A SystemFragmenter we got from somewhere
AMol=System()#A molecule we got from somewhere

#Fragment the system, result is a dictionary from serial number to NMerInfo
Frags=MyFragger.fragmentize(AMol)

#The number of subsystems
Frags=Frags.size()

for SNi,Fragi in Frags.items():
    SNi#The serial number of the subsystem
    Fragi#The NMerInfo struct
    SNi_2=Fragi.SN#The serial number is stored here too
    ci=Fragi.weight#The coefficient
    Fi=Fragi.NMer#The actual subsystem
~~~


