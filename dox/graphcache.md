The Graph and Cache                               {#graphcache}
===================

The Graph
---------

\todo Fill in graph information


Data Caching
------------

Each module (whose information is stored in the
ModuleManager/ModuleAdministrator) has a *cache* associated with it.  This cache
is an instance of the CacheMap class. The purpose of this cache is to prevent
duplication of work should a function in the module be called with the same
inputs. How a module utilizes its cache is completely up to the module writer; 
blame them if they don't use it correctly.  Additionally, the cache represents
the key to checkpointing and extending computations, thus it is in the module
writer's best interest to understand and utilize the cache.

It should be noted that a module may only access its own cache, and
therefore it cannot be used as a data-passing mechanism between modules,
which would bypass the module interface.  Put another way, the cache is only
global for a particular module (specifically a module type, all module keys
associated with a module type have the same cache instance).

Things that are expected to be stored in the cache are larger objects such as 
matrices and tensors, but anything can be stored (such as normalized basis sets 
or transformed molecules).

Indexing in the cache
---------------------

The cache is a map of keys to arbitrary data types. However, specifying
what the key is can be tricky. There are two components to the key. First,
is an arbitrary, descriptive string which is used only by the module
writer. The second is a string that is meant to provide a concise, unique
identification of exactly how this data was (or will be) generated.  Rather than
using a string like "Water hexamer with coordinates at..." what you will usually
want is a hash value of all variables that determine how to generate the piece
of data.  (Nearly) every class in the core of Pulsar is capable of generating
a hash representation of itself, so simply combine these hashes to get the final
string.

One notable complication to this, however, is that many of the options in the
OptionMap may have no effect on the data (ie, printing, etc). Therefore, only
 \b significant options should be hashed. These significant options take
the form of a set of strings which specify which option keys are to be
compared. If all the options represented by the significant option keys
are equal, then the data is assumed to be what is requested. Ensuring
that all the necessary options are part of the significant options is also
on the module writer. Although annoying, there is no other way to do it.
By default, all options will be checked.


Using the Cache
---------------

The typical workflow should be, for expensive operations, to hash all input
variables and then check if that hash is in the cache. If not, then you should
calculate the requested operation, store the result in the cache, and finally
return the data.  This effectively moves responsibility of checking if data
has already been computed closer to the actual generation of the data,
where, in our opinion, it should be.  This also encapsulates this on a module
by module basis.  Just because two modules are both Hartree-Fock modules doesn't
mean they depend on exactly the same input variables.

As a small example, it is no longer the responsibility of a Hartree-Fock
module to determine if integrals have already been computed. To do
it that way would require logic in the HF code for determining if the
integrals were computed with the right molecule, basis set, options,
etc. This logic would then be duplicated across all similar modules.
Furthermore, the code would be fragile in that changing options required
by the integral code (for example, by introducing screening) may require
changing all the places where the existance of integrals is checked.  Moving the
checking to the actual integral code removes this coupling. Therefore, if our HF
code requires integrals, it always creates an integral module and requests the
integrals.  The module then checks if they have already been computed and if so,
returns (relatively) instantly. If not, it calculates them.

Practically speaking, a Cache instance is used like:
~~~{.cpp}
//You will always get the cache from the ModuleBase your module implements
CacheData& MyCache=this->cache();

//Determine what influences the value of the data
bphash::HashValue MyHash;

//We'll pretend it depends on the System that was passed in, which we called Mol
MyHash=Mol.my_hash();

//...and some options called option1 and option2
OptionMap& MyOptions=options();//Your options are stored in the ModuleBase
bphash::HashValue OptHash=MyOptions.hash_values({"option1","option2"});

//For the moment just concatenate the string representations
std::string StringHash=bphash::hash_to_string(MyHash)+
                       bphash::hash_to_string(OptHash);

//Try to get the data
std::shared_ptr<CoolDataType> Data=MyCache.get<CoolDataType>(StringHash);

//One of two things happend
if(Data){//The cache had the data
   return *Data;//or do whatever it is you have to do with the data
}
else{//The cache did not have the data, generate it

CoolDataType TheData;//Assume this is the generated data

MyCache.set(MyHash,TheData,Policy);//Save it (policy explained below)
return TheData;//or again do whatever it is you have to do with the data
}
~~~


Storing the Cache
-----------------

Checkpointing of the cache is tricky, although should not be impossible.
For now, the discussion of checkpointing has been put on the backburner.

\todo Checkpointing 
