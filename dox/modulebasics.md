# Basic Structure of Modules          {#modulebasics}

The core of the project is the concept of a *module*. A module is
a piece of runtime-loadable code used to add functionality. In more general
terms, a module is a plugin.

Modules within the Pulsar framework have a predefined interface for a given type of
task. These general tasks are the module's *base type* or *base
class*. Base types can be, for example, FourCenterIntegral or
TwoCenterIntegral. They specify the overall purpose of the module.

Since several modules are often related, and often share some code
between them, modules are grouped together into a *supermodule*.
Simply, a supermodule is a collection of related modules. All modules
must be part of a supermodule, even if the supermodule only contains that
one module. 

In more concrete terms, a module is a class derived from an (abstract)
module base class that implements the methods required from that class.
Modules and supermodules may be written in C++ and/or Python and can
can be built completely outside of the Pulsar source tree,
utilizing only the installed headers. Explicit linking to the core
is not necessary.

The process of *loading* a module means opening the supermodule file
or python package and extracting information about the modules within
it and how those modules can be created. The information for the desired
module is then stored for use by other modules.
On the other hand, *creating* a module means using that stored
information to create or instantiate an object of that module.

## A note about the structure of a supermodule

A supermodule is, in practice, a Python package. Therefore, it must include
an `__init__.py` file. The supermodule is imported by Pulsar through Python's
native `import` mechanism. Initialization code for the supermodule can be
placed within the `__init__.py` file (or called from there).



