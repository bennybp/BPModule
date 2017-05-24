Getting Started
===============

In a typical workflow the input to Pulsar will be a series of Python commands.
It is thus assumed that the reader has at least a passing familiarity with how
to use Python.


The "Hello World" of Pulsar
---------------------------

\note Python looks for Python modules in a few standard places (its install
directory,
the current directory, and any paths contained within the environment variable
`PYTHONPATH`).  If you followed the build directions given in
[Compiling and Installing Pulsar](@ref building) then Pulsar will be installed
in `/CMAKE_INSTALL_PREFIX/lib/pulsar/modules`.  You will need to add this path
to your `PYTHONPATH` environment variable so that Python can find it.

Assuming Pulsar is installed in a location known to Python, the most basic
usage of Pulsar is:

~~~{.py}
import pulsar as psr
import sys

psr.initialize(sys.argv)
psr.finalize()
~~~

If this script is stored in a text file named `MyPulsarScript.py`, Pulsar can be
run via:

~~~{.sh}
$python MyPulsarScript.py
~~~

This script does nearly nothing.  The first two lines make Python aware of the
Pulsar module and the sys module (a standard Python module for obtaining
information about the computer that Python is being run on).  As suggested by
the function names, the last two lines will initialize and finalize Pulsar.
Initialization/finalization in Pulsar primarilly consists of
initializing/finalizing MPI as well as redirecting output buffers.  consists by
passing it the command line arguments (in Python `argc` is obtained via
`len(sys.argv)`) and then finalize Pulsar.  This script is not very interesting,
but it provides a starting point for all other Pulsar runs as these four
commands will always be needed.

The "Hello World" of Pulsar (that actually does something)
----------------------------------------------------------

Using the above script as a scaffolding, let's introduce the next layer of
complexity, specifying the molecule we want to run.

~~~{.py}
import pulsar as psr
import sys

psr.initialize(sys.argv)
h2 = psr.make_system(
"""
H 0.0 0.0 0.89
H 0.0 0.0 0.0
"""
)

psr.finalize()
~~~

The above script creates an hydrogen molecule with a bond length of 0.89
\f$\AA\f$ aligned with the Z-axis.  What's cool about this is example is that
the value `h2` is actually a Python object.  Specifically it is an instance of
the Pulsar System class.  The System class has its own member functions and
state and all of them are callable from this input script.  For example, we
can loop over the atoms in our hydrogen molecule and print them out by:

~~~{.py}
for atom_i in h2:
   print(atom_i)
#BTW, to print the molecule we could have just done print(h2)
~~~

A more complete tutorial on using the System class is beyond the scope of this
simple introduction, but interesed readers will find such a tutorial available
[here](@ref systems).

Now that we have our system we need to apply an atomic basis set to it.  Pulsar
comes with a library of common basis sets (*e.g.* aug-cc-pVDZ, STO-3G, *etc.*)
that can be applied to a system via the `apply_single_basis_set` function.  For
example to apply the `aug-cc-pVDZ` basis set to our hydrogen molecule the
command is:

~~~{.py}
import pulsar as psr
import sys

psr.initialize(sys.argv)
h2 = psr.make_system(
"""
H 0.0 0.0 0.89
H 0.0 0.0 0.0
"""
)

h2_with_basis = psr.apply_single_basis_set("PRIMARY","aug-cc-pvdz",h2)
psr.finalize()
~~~

Pulsar allows a single molecule to have many basis sets.  Each basis set is
assoicated with a unique name (in this case we have chosen the name "PRIMARY").
This name is somewhat arbitrary and largely serves only to provide a unique
identifier for the basis set.

We anticipate that most pulsar runs will want to compute the energy of a
chemical system using some method (*e.g.* Hartree-Fock, QM/MM, *etc.*).  In
Pulsar such methods are implemented as objects derived from the class
`EnergyMethod`.  The input to all of these methods is the derivative order and
the wavefunction.  The system is passed to the method via the fact that it is
one of the wavefunction's members.  Thus our next task is to make a wavefunction
that has our hydrogen molecule as a member.  This can be done via:

~~~{.py}
import pulsar as psr
import sys

psr.initialize(sys.argv)
h2 = psr.make_system(
"""
H 0.0 0.0 0.89
H 0.0 0.0 0.0
"""
)

h2_with_basis = psr.apply_single_basis_set("PRIMARY","aug-cc-pvdz",h2)

wfn = psr.Wavefunction()
wfn.system = h2_with_basis
psr.finalize()
~~~

Pulsar by default does not have the ability to run computational chemistry
computations.  You will need to install modules to do that.  To complete this
tutorial let's assume you have installed the supermodule `best_methods` which
contains the Pulsar SCF module `best_scf`.  To use our wavefunction with this
method the final script is:

~~~{.py}
import pulsar as psr
import sys

psr.initialize(sys.argv)
h2 = psr.make_system(
"""
H 0.0 0.0 0.89
H 0.0 0.0 0.0
"""
)

h2_with_basis = psr.apply_single_basis_set("PRIMARY","aug-cc-pvdz",h2)

wfn = psr.Wavefunction()
wfn.system = h2_with_basis

with psr.ModuleAdministrator() as mm:
    mm.load_module("best_methods","best_scf","SCF")
    scf = mm.get_module("SCF",0)
    newwfn, egy = scf.deriv(0,wfn)

psr.finalize()
~~~

Respectively the additional four lines create an instance of the Pulsar
framework, load `best_method`'s SCF module into the framework under the
key `SCF`, get the module with key `SCF`, and compute the energy (0-th
derivative) using our wavefunction.  The result of the computation is a new
wavefunction (which we save to a variable `newwfn` and the energy (which we is
in the variable `wfn`.

\note The Python `with as` statement is a way to control the lifespan of an
instance.  In this case we ensure that ModuleAdministrator has the chance to
unload any modules it loaded before it is destroyed.  Without the `with as`
statement the destruction order would be dictated by Python's garbage collector
which is non-deterministic.

Where to go from here
---------------------

This tutorial was meant to give you a taste of how Pulsar works.  Where you go
from here depends on whether you are a user or a developer.

TODO: Finish when logical additional documentation is available.
