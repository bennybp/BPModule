Using and Setting Options                                   {#Options}         
=========================

Electronic structure codes have many options available to them and managing them
can be a nightmare.  Thankfully, BPModule has a plan, which is detailed in this
section.

OptionMap
----------

The object that actually holds the options is of type OptionMap.  For the most
part a module's interactions with this object will be of the form:

~~~{.cpp}
const OptionMap& Op=Options();
std::string SomeStringOption=Op.Get<std::string>("KEY_FOR_STRING_OPTION");
int SomIntOption=Op.Get<int>("KEY_FOR_INT_OPTION");
double SomDoubleOption=Op.Get<double>("KEY_FOR_DOUBLE_OPTION");
~~~

The OptionMap class also has functions for determining whether or not an option
has been set by a user and/or whether the option has its default value (note
that the user can set a value to its default value), which may prove useful
to some modules.

Setting Options in the Input Script
-----------------------------------

For the most part all option modification should occur within the input
script.  Say we are setting the options for the module with the key "MY_MODULE".
This is done after loading the module into the module manager, but before 
it is actually used.  Specifically:
~~~{.py}
#assume mm is an instance of a ModuleManger (technically ModuleAdministrator,
#but the difference is immaterial presently)
mm.LoadModule("SUPERMODULE_FOR_MY_MODULE",
              "MODULE_NAME_FOR_MY_MODULE",
              "MY_MODULE")
mm.ChangeOption("MY_MODULE","OPTION_NAME",value)
MyModule=mm.GetSubmodule("MY_MODULE",0)
~~~

Setting Options in a Module
---------------------------
It should be a somewhat rare occurence that primarily manifests for 
"driver-like" modules, but it is possible to set options within a module for
a child module.  Assuming we want to change an option for the module with the
key "OLD_KEY".  In C++ (assuming mm is a ModuleManager instance):

~~~{.cpp}
mm.DuplicateKey("OLD_KEY","NEW_KEY");
mm.ChangeOption("NEW_KEY","OPTION_NAME",value);
~~~

Or in Python:
~~~{.py}
mm.DuplicateKey("OLD_KEY","NEW_KEY")
mm.ChangeOption("NEW_KEY","OPTION_NAME",value)
~~~

As you can see this procedure is similar to setting options in the input script
except that LoadModule is replaced by DuplicateKey.  Within a module you should
never modify the options of a key you did not generate.

Defining Options for a Module
-----------------------------
There are two primary manners by which options for modules are set.  The first
is done in the base module for your module type.  For example, the EnergyMethod
base module predefines the following options:

- MAX_DERIV (the highest analytic derivative available)
- BASIS_SET (the key for the basis set to use)
- E_TOLERANCE (the tolerance to which the energy should be converged)

Simply by deriving from the EnergyMethod base module your module will have these
options available to it.  All other options that your module has should be in
its `modinfo.py` file.  Note that you can override any of these base options
within your `modinfo.py` file, e.g. if you don't have 255-th order derivatives
you should change the default value for MAX_DERIV.

Naming Conventions
-------------------
In an attempt for some uniformity we have provided some suggested conventions
for naming your options.  

TODO: Probably just steal Psi4's