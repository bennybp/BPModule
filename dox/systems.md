How to Make and Use Systems                                          {#systems}
===========================

This page is aimed at giving you a crash course in working with the Pulsar
System.  If there is a task you commonly perform with a system not covered
here feel free to add it.

## Systems as Implemented in Pulsar Overview

The entirety of electronic structure theory revolves around computing properties
for some molecular system.  For our purposes a molecular system consists of the
atoms, basis sets associated with those atoms, fields surrounding the atoms, and
the periodicity of the system.  Notably this means we do not have both a
molecule and basis set class.  Ultimately this decision stemmed from noticing
patterns in the code, specifically that one usually wants to treat atoms and
their basis sets in a unified manner (e.g. moving an atom, means moving its
basis functions, equivalence of two atoms requires they have equivalent basis
sets, etc.).  Although this is non-standard, I think once you start using it
you'll realize this simplifies things quite a bit.

Another slightly odd thing about our system class is its set like nature.
Many system manipulations can be thought of as set operations.  For example
combining two molecules requires taking the union, list of all elements common
to two molecules is an intersection, atoms present in one, but not the other is
a set difference, etc.  Furthermore for methods that rely on many systems, such
as the many-body expansion, storing all of these systems becomes expensive as
does manipulating them.  For this reason having only one copy of an atom that
actually lives in the universe dramatically speeds things up.  Conceptually each
system now contains pointers to the atoms it contains. If you are actively 
manipulating systems then you'll need to be aware of this fact because 
attempting to add an atom to a system only works if that atom is also in the
universe.  If you are simply using a system that has been given to you the
underlying universe is not important.

The final point to be aware of is that the System is designed to be used in a
truly object-oriented manner in the sense that it expects to work with objects
not integers.  Things like add atom 2 are frowned upon; instead one actually
creates the atom they want and then inserts it.  Similarly there is a focus
on returning associated objects like a distance matrix in associative arrays.
For example, the distance matrix takes a pair of actual atoms (not their 
indicies) and returns the distance between them.

## Making Systems

To make a system the first thing you have to do is make a
pulsar::math::Universe<Atom>, a type we have aliased to AtomSetUniverse.
The universe needs to have all atoms that you may possibly want to put in your
system.  The system will then include subsets of that universe (or the entire
universe, which is technically a subset of itself).   Usually this will just be the atoms in
the input file, but it can also include other "atoms" like point charges and
ghost atoms.  More on that later.  For the time being note it is as simple as
(in C++):

~~~{.cpp} 
pulsar::system::AtomSetUniverse MyNewUniverse;
MyNewUniverse.insert(pulsar::system::CreateAtom({x,y,z},Z)); 
~~~

or in Python:

~~~{.py} 
MyNewUniverse=pulsar.system.AtomSetUniverse()
MyNewUniverse.insert(pulsar.system.CreateAtom([x,y,z],Z)) 
~~~

In either case we are creating an atom with the given Cartesian coordinates (should be in a.u.), and an 
atomic number of Z.  One
continues to add atoms in this fashion until all desired atoms are in the 
Universe. Next, we make a system that is associated with that universe in (C++):

~~~{.cpp}
pulsar::system::System MyNewSystem(MyNewUniverse,true);
~~~

or in Python:

~~~{.py}
MyNewSystem=pulsar.system.System(MyNewUniverse,True)
~~~

in both cases the true argument tells our system to go ahead and populate the
system with all the atoms in the universe.  If we had used false instead then
our system would have no atoms yet.
