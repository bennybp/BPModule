Terminology                                                       {#terminology}
===========

Much of the documentation of Pulsar uses a language we have agreed upon to
describe key concepts.  These terms should hopefully be self explanatory without
this page, but for reference, they are included here.

- Framework: The scaffolding on which a computer program is built.  Programs
  usually start from a framework to avoid having to reimplement much of the
  low-level routines.  Presently, what we call Pulsar core is the framework.
  Starting from the core it should be relatively easy to build a computational
  chemistry package in a highly customizable manner and rapidly by leveraging
  existing modules (defined below) to perform tasks you don't want to code up.

- Module Manager: This is an instance of the module manager class that is
  capable of giving you a module when asked.  For all intents and purposes you
  can think of the module manager as the framework.

- Module: Pulsar can be thought of as a program with missing code.  At
  select locations in the program it needs to do some abstract task, but doesn't
  actually have the code to do it.  Instead, it knows that if it say needs an 
  energy so it calls something that will give it one (called an EnergyMethod).
  The something it calls is a module.  Modules are written in C or Python.  If
  they are in C then they are shared libraries.  If they are in Python they are
  Python modules.  When Pulsar calls a module it will automatically do so
  in that module's native language.  It also will take any returns from the
  module in that module's native language.

- Module Base:  This is a bit more abstract.  Module bases are abstract base
  classes that define the API a module must satisfy to be used in a particular
  manner.  For example a library that computes the SCF energy must satisfy the
  EnergyMethod API (EnergyMethods are functions that take wavefunctions and 
  compute energy derivatives it).  Modules actually implement the functions in 
  the module base API.

- Super Module: We anticipate that most groups won't just write one module and
  call it quits.  Rather they will write a series of modules.  These modules
  may use common components among themselves which are not part of Pulsar.  For
  example maybe they all use some tensor libray.  A super module is just a
  collection of modules that are designed to be used together for maximum impact.
  It usually will be name of the library or Python module that contains all of
  the modules.

To some extent a super module is a namespace and a module is a class in that
namespace.  One of the first packages interfaced with Pulsar is Psi4 so we use
this as an example for the moment.  Psi4 is the super module, within this super
module are a series of modules, which are things like DF-HF, DF-MP2, canonical
HF, etc.  Essentially each algorithm is its own module, but that doesn't quite
define the algorithm, for example we need to know the convergence thresholds of
the SCF.  This leads us to:

- Module Key:  An instance of a module.  Associated with this instance are a set
  of options.  These are what are actually dealt with throughout the framework.
  When you need to get a module you give the module manager the key and it
  returns the module.  Your code doesn't actually need to know the super module
  or module at all (it does have to know the module base though).  E.g. MP2
  routines will have an option along the lines SCF_KEY, which is the module key
  that it should use when it wants to get the SCF reference.  One can change
  the options to the SCF that MP2 module will use simply by changing SCF_KEY.
