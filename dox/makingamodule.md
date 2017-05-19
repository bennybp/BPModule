Creating Modules, SuperModules, and Module Base Types        {#makemod}
======================================================

The point of this page is to get you started in creating your first module and 
to document the steps required to make super modules and module base.  We 
anticipate that creating modules will be the most common scenario bringing users
 to this page followed by creating super modules.  Most users will never have to 
write a module base, but it is included here in case you want to extend the framework.  

The steps to making a module can roughly be described as:

1. Determine what module base type your module will derive from
   - Consult the modulebase directory of Pulsar for a list of modules
2. Decide on which super module it will belong to
   - All modules must belong to a super module, even if that module is the only 
     module in the super module
   - The choice of super module, for the most part, will be dictated by your 
     affiliation, e.g. Psi4 modules are part  of the Psi4 super module, etc. 
   - Ususally the name of the super module will be the name of the library (for
     C modules) or the name of the Python module (for Python modules)
3. Write the module info file
4. Write your module
5. Build and install your module
6. Run your module

## Making Your Module ###

### C++ Instructions ###

In C++, simply derive from the module base type you are implementing.  Each 
module base type defines the minimal API that your module must implement in 
order to work and the conventions that must occur at the interface.  For example,
say you are implementing a new method that computes an energy.  The appropriate 
class to inherit from is `EnergyMethod` and the code, in your header
file looks like:

~~~{.cpp}
    class MyNewMethod: public pulsar::modulebase::EnergyMethod{
       public:
       //Only use the constructors defined in the base class, or else your 
       //module can not be loaded
       using pulsar::modulebase::EnergyMethod;
       
       //We need to implement the Deriv_ function (exact requirements can be  
       //found in the module base type headers)
       std::vector<double> Deriv_(size_t i);
       };
~~~

In your .cpp file you would then write out the Deriv_ function's guts.

### Python Instructions ###
Like C++, in Python you need to derive from the base type you are implementing.  
Again if we were implmenting a new method for computing energies:

~~~{.py}
   class MyNewMethod(psr.modulebase.EnergyMethod):
     def __init__(self,myid):
        super(MyNewMethod,self).__init__(myid)

     def Deriv_(self,order):
        #Compute energy here
~~~

## Building Your Module ###
For the most part you are free to configure your build however you like 
(although we strongly recommend you use CMake otherwise dependency forwarding 
will quickly become a nightmare).  Building really only applies to C++ modules.  
Python modules simply need installed.

### CMake and Installation ###

A typical `CMakeLists.txt` for a module looks like::

~~~{.sh}
    #These two commands would occur in your top-level CMakeLists.txt 
    #before including any folders
    #they find the pulsar core and tell your modules to use its header files
    find_package(pulsar REQUIRED)
    include_directories(${PULSAR_CXX_INCLUDES})

    #These are the commands for actually building your module
    #Note that "YourModulesName" is being used as the target name and a 
    #folder namespace
    set(YOUR_MODULES_SRC_FILES file1.cpp file2.cpp ...)
    add_library(YourModulesName MODULE ${YOUR_MODULES_SRC_FILES})
    target_link_libraries(YourModulesName pulsar)
    install(TARGETS YourModulesName LIBRARY DESTINATION YourModulesName RUNTIME DESTINATION YourModulesName)
    install(FILES "__init_.py" DESTINATION YourModulesName)
    install(FILES "modinfo.py" DESTINATION YourModulesName)
~~~

Notes:
- Be sure to specify your compilers!
- CMake only needs to include the `pulsarConfig.cmake` file (which is looked for 
  by `find_package`). 
- The modules are built as MODULE types
- The modules must be linked to the interface target `pulsar` plus any libraries 
  specific to the module.
- Installation in the above goes to `${CMAKE_INSTALL_PREFIX}/YourModulesName`
  - It doesn't matter really where you install it, but the ModuleManager's paths
    will have to be updated accordingly 
- Don't forget to install `__init__.py` and `modinfo.py`, plus any other python files!
- Modules are meant to be built against an installed pulsar core, not the build directory. 
  - No idea if building against `<builddir>/stage/<path>` will work.
- Modules are meant to be run only after installation as well. They should be
  tested in the build directory

## Creating a Supermodule ##

### Parts of a Supermodule ##

Every supermodule requires the three following files

  - **modinfo.py** (required) - Information about the supermodule and its modules
     - Should contain a dictionary with entries for each module in the supermodule
     - Each entry maps a string, the module name, to a dictionary of information
       about that module, specifically:
       - "type" either "c_module" or "python_module"
       - "modpath" the path to the dynamic library
          - Library is relative to the directory in which the __init__.py is
            installed
       - "version" the version...
       - "description" a description...
       - "authors" who wrote this module
       - "refs" what should be cited when the module runs
       - "options" a dictionary of option names as strings to a tuple that contains
          - The type of the value (string, float, etc.)
          - The default value (use None for no default)
          - True/False does the option need to be set before the module is called
          - Any checks that should be run on the value (ensure it's greater than 0, etc.)
          - A short descriptive string of what the option does         

  - **__init__.py** (required) - Needed for importing your supermodule. 
     - Used to initialize your supermodule
     - Must minimally contain `from .modinfo import *`
  - A file that creates 
     - In C++ you need to compile:

~~~{.cpp}          
           extern "C" {
              ModuleCreationFuncs InsertSupermodule(void){
                 ModuleCreationFuncs cf;
                 cf.AddCppCreator<type of your module>("module name");
                 return cf;
              }
~~~
       - Currently this file is being called `creator.cpp`
     - In Python the file looks something like::

~~~{.py}       
           from pulsar.modulemanager import ModuleCreationFuncs

           def InsertSupermodule():
               cf = ModuleCreationFuncs()
               cf.AddPyCreator("module name",an instance of module)
               return cf
~~~
       
       - Currently this file is being called `creator.py`
       - InsertSupermodule/InitializeSupermodule/FinalizeSupermodule must be 
         part of __init__.py or imported from there

C++ and Python Supermodules can be part of the same directory, with some care.


## Creating a Base Module Type ##

Base module types are always in C++ and mirrored in Python.  This ensures
maximum performance is possible for performance critical modules.  Base module 
types have a few catches that normal modules don't.  Specifically they have to
also implement the machinery necessary for the module manager to create them.  
Briefly these considerations are:

- Constructor needs to have the signature and definition:

~~~{.cpp}
        MyBaseModule(unsigned long id):ModuleBase(id,"MyMethod"){}
~~~
- A typedef of its type to "BaseType", i.e. `typedef MyBaseModule BaseType;`
- All methods that are virtual need wrapped in two ways (one for C++, the other
  for Python)
  - Wrapping allows the `ModuleManager` instance to downcast 
~~~{.cpp}
        //The function derived classes override
        virtual return_value MyVirtualMethodImpl(Arg1,Arg2,...)=0;

        //The public interface to that function
        return_value MyVirtualMethod(Arg1,Arg2,...){
             return ModuleBase::CallFunction(&MyBaseModule::MyVirtualMethodImpl,
                                             Arg1,Arg2,...);
        }

        //In a new class
        class MyBaseModule_Py : public MyBaseModule{
        public:
            //Needs same constructor
            using MyBaseModule::MyBaseModule;

            \todo document this macro and figure out if all base modules need it
            MODULEBASE_FORWARD_PROETECTED_TO_PY
         
            //Register the virtual types with Python
            virtual return_type MyVirtualMethodImpl(Arg1,Arg2,...){
                return CallPyOverride<return_type>("MyVirtualMethodImpl",
                                                    Arg1,Arg2,...);
            }
        };
~~~

# Troubleshooting
## `dynamic module does not define module export function`

Typically this error looks like:

~~~
pulsar.exception.PulsarException.PulsarException: Unable to load supermodule
             supermodule : <supermodule_name>
               exception : dynamic module does not define module export function (PyInit_<supermodule_name>)
~~~

Common causes:
- The name of the library does not match the supermodule name
- In `__init__.py` you are trying to import a pure C++ library
