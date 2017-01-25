How the ModuleManager Actually Works
====================================

Let's start with C++ modules.  In C++ all modules are a class that ultimately
derives from `ModuleBase`.  They are required to have a constructor that takes
an argument of `ID_t`, which will be the ID number of the module for the run.
Consequentially, we know that to instantiate a module of type `T`, the call
is:

~~~.cpp
ID_t ID;
T my_module(ID);
~~~

The next task is to create a functor that when called returns an object of type
`T`.  This is handled in `ModuleCreationFuncs.add_cpp_creator<T>` and is why all
supermodules must contain a ModuleCreationFuncs instance.  The
MoculeCreationFuncs class is a thin wrapper around a map from module types to
functors, where the functor returns an object of type `T` given an `ID_t`.

Python modules behave similarly.  Specifically they all must define a `__init__`
function that takes the ID number of the module.  They are then registered with
the `ModuleCreationFuncs.add_py_creator` member function, which creates a
functor that will return a `pybind11::object` instance in C++ or the module in
Python.

