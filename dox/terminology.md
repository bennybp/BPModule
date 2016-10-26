Terminology                                                       {#terminology}
===========

Pulsar's documentation uses a mix of actual computer science terminology and
terminology invented for Pulsar itself.  The next sections are designed to
provide a glossary of the important terms and serve as a self contained
reference for new users and developers.

## Computer Science Terminology

As a computer program, Pulsar relies heavily on established computer science
ideas.  Particularly those of the object-oriented programming style.  We expect
that users and developers are already familiar with many if not all of these
terms, but in case there is some ambiguity in what we mean by a term we have
included the key concepts here.

- Resources : These are things like RAM, hard disk space, bandwidth.  They are
  things your program requires of the computer or of the universe to run.  If
  it doesn't have enough of these things the progam either can't run or will run
  for a very long time.

- Types : What kind of data something actually is, i.e. is it an integer
  (typically called an int), is it an English word (called a string), is it
  a real number (called a float or a double depending on how many digits it
  stores), etc.  Types that literally do nothing but store data are often called
  plain ol' data (POD) and those that are not are...

- Class : These are non-POD types that are often domain specific.  For example
  we have a System class to describe the molecule we are running a computation
  on.  Unlike POD types, classes have actual code in them, which may include
  the algorithms for setting them up, freeing any resources they require, and
  functions to perform certain tasks, *e.g.* System has a function that will
  compute its mass.

- Class instance : This is an actual "value" of a class.  For example the System
  class is the same thing regardless of whether the data inside of it is
  consistent with that of hydrogen or DNA.  Hydrogen and DNA are two instances
  of the System class.

- Member function : A function that is part of a class

- Member data (or attribute) : A POD value or class instance that is part of a
  class

- API : stands for application programming interface, but the initialism is 
  becoming a word in and of itself.  This is the code interface that users
  interact with to get the program to do tasks for them.


## Pulsar Specific Terminology

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
