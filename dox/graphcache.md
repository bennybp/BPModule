The Graph and Cache                               {#graphcache}
===================

The Graph
---------

\todo Fill in graph information


Data Caching
------------

Each module (whose information is stored in the
ModuleManager/ModuleAdministrator) has a *cache* associated with it. The
purpose of this cache is to prevent duplication of work should a function
in the module be called with the same inputs. How a module utilizes its
cache is completely up to the module writer; blame them if they don't
use it correctly.

It should be noted that a module may only access its own cache, and
therefore it cannot be used as a data-passing mechanism between modules,
which would bypass the module interface.

This that are expected to be stored in the cache are larger objects such as matrices and tensors,
but anything can be stored (such as normalized basis sets or transformed molecules).

Indexing in the cache
---------------------

The cache is a map of keys to arbitrary data types. However, specifying
what the key is can be tricky. There are two components to the key. First,
is an arbitrary, descriptive string which is used only by the module
writer. The second are the options used in the generation of the data
that is stored (or is going to be stored).

One complication of this, however, is that many options may have no effect
on the data (ie, printing, etc). Therefore, only \b significant options
should be compared when requesting data. These significant options take
the form of a set of strings which specify which option keys are to be
compared. If all the options represented by the significant option keys
are equal, then the data is assumed to be what is requested. Ensuring
that all the necessary options are part of the significant options is also
on the module writer. Although annoying, there is no other way to do it.
By default, all options will be checked.


Using the Cache
---------------

The typical workflow should be, for expensive operations, to first
check if the data is in the cache. If not, then you should calculate it,
of course.  This effectively moves responsibility of checking if data
has already been computed closer to the actual generation of the data,
where, in our opinion, it should be.

As a small example, it is no longer the responsibility of a Hartree-Fock
module to determine if integrals have already been computed. To do
so that way would require logic in the HF code for determining if the
integrals were computed with the right molecule, basis set, options,
etc. This logic would then be duplicated across all similar modules.
Furthermore, the code would be fragile in that changing options required
by the integral code (for example, by introducing screening) may require
changing all the places where the existance of integrals is checked.

Moving the checking to the actual integral code removes this
coupling. Therefore, if our HF code requires integrals, it always creates
an integral module and requests the integrals.  The module then checks if
they have already been computed and if so, returns (relatively) instantly.
If not, it calculates them.


Storing the Cache
-----------------

Checkpointing of the cache is tricky, although should not be impossible.
For now, the discussion of checkpointing has been put on the backburner.

\todo Checkpointing 