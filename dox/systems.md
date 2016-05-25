#How to Make and Use Systems ##  {#systems}

The entirety of electronic structure theory revolves around computing properties for some molecular system.  For our purposes a molecular system consists of 
the atoms, basis sets associated with those atoms, fields surrounding the atoms,
and the periodicity of the system.  Notably this means we do not have both a molecule and basis set class.  Ultimately this decision stemmed from noticing patterns in the code, specificially that one usually wants to treat atoms and their basis sets in a unified manner (e.g. moving an atom, means moving its basis functions, equivalence of two atoms requires they have equivalent basis sets, etc.).  Although this is non-standard, I think once you start using it you'll realize this simplifies things quite a bit.

The other slightly odd thing about our system class is its set like nature.  Many system manipulations can be thought of as set operations.  For example combining two molecules requires taking the union, list of all elements common to two molecules is an intersection, atoms present in one, but not the other is a set difference, etc.  If you are actively manipulating systems then you'll need to be aware of this fact.  If you are simply using the one that has been given to you it's not so important.

## Making Systems

To make a system the first thing you have to do is make a pulsar::math::Universe<Atom>, which can be thought of as all possible atoms you may want to use to build your molecule.  Usually this will just be the atoms in the input file, but it can also include other "atoms" like point charges and ghost atoms.  More on that later.  For the time being note it is as simple as (in C++):

~~~{.cpp}
using pulsar::system;
AtomSetUniverse MyNewUniverse;
MyNewUniverse.insert(CreateAtom(0,{x,y,z},Z));
~~~

or

~~~{.py}
MyNewUniverse pulsar.system.AtomSetUniverse()
MyNewUniverse.insert(CreateAtom(0,[x,y,z],Z))
~~~

In either case we are creating an atom with index 0 (the ability to specify indices will likely go away in the future), the given Cartesian coordinate, and an atomic number of Z.  Of course you 
