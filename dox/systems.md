How to Make and Use Systems                                          {#systems}
===========================

In Pulsar we term the entire system of interest, the molecule, its atoms, its
basis functions, any fields it is embedded in, etc. the *system*.
Consequentially, the System class is your main source of information about the
chemistry the user is trying to model.  This page walks you through the parts
of the Pulsar System class and also covers basic system manipulations.

## The Atom Class

Arguably the lowest level part of the system is the atoms.  In Pulsar each
atom is responsible for holding information pertaining to the nucleus and the
electrons.  The majority of the class is autopopulated for you if you use one of
the factory functions provided.  These are functions like:
~~~{.cpp}
//Makes a hydrogen atom at the origin
Atom H=pulsar::system::create_atom({0.0,0.0,0.0},1);

//Makes a deuterium atom at the origin
Atom D=pulsar::system::create_atom({0.0,0.0,0.0},1,2);
~~~

When you use these functions the mass, charge, multiplicity, number of elctrons,
etc. are auto filled in for you.  You are free to change any of the values,
after instantiation if the values do not suite your needs.

Pulsar has support for several types of pseudoatoms.  Likely one of the more
common pseudoatoms is the *ghost* atom, which has all the basis functions of a
particular atom, but no nucleus or electrons.  