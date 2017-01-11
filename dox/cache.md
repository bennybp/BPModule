The Cache Concept in Pulsar                                             {#cache}
===========================

One of the tricks to a framework like Pulsar is how to pass data between
modules.  This is a complicated problem because even though two modules both
compute an energy, for example, they may require very different inputs in order
to do this.  Pulsar works by insisting that all modules that compute an energy
use the same API, hence having a custom API breaks Pulsar.  For small
differences in input parameters, like setting an algorithm detail (defining
convergence, number of iterations, etc.) the OptionMap concept suffices.  For
larger quantities, like integrals, it does not.  We thus need a way to "pass"
quantities like integrals between modules.  Pulsar's solution is conceptually
simple, if you need say integrals call an integrals module and use the result.
From the perspective of your module you are not passing the integrals instead
you are computing them when necessary.  Similarly, if you need a set of
molecular orbitals call an SCF module.  If you need to break the system into
pieces call a system fragmenter, etc.  Exactly which module to call can be set
at runtime by passing the module's key as an option.  Ultimately, this means
any module can get whatever data it needs, whenever it needs it without having
a custom API.

Depending on what the submodules are computing, letting them actually recompute
the quantity of interest is not always practical.  For example, in the course of
an MP2 computation both SCF and MP2 would call the integrals module (SCF calls
it to build the Fock matrix, MP2 calls it for the integral transformations).
Most of the time we do not want to regenerate the integrals given their cost.
This is where the cache comes in.  The integrals module looks at the
input it was given (the basis set, the molecule, and the various threshold
options) and generates a string unique to this information (will usually be a
hash value *vide infra*)  it then checks the cache for data indexed under this
string.  If such data exists, the module returns it, otherwise it generates the
data, puts it in the cache, and then returns it. Consequentially, cached results
are only computed once, even if the module is called multiple times with the
same input.

As the previous paragraph suggests, the cache can be thought of as a giant
dictionary/map between a computation and its results.  However, as implemented
in Pulsar, the cache is quite a bit more powerful.  This page is designed to
familiarize you with the basics of the cache and how to use it effeciently.

CacheMap Class in Pulsar
-----------------------

The cache is an instance of type CacheMap.  Each module type whose information
is stored in the ModuleManager/ModuleAdministrator has a cache assigned to it.
Again the cache is assigned on a module type basis **not** on a module key
basis.  Thus a CacheMap instance is accessible from all instances of a module
type and can be used to pass data among them.  However, because each module
type has a different cache, the CacheMap class can **not** be used to pass data
among different module types.  Put another way, each module type's cache is
global data for that type, but is not global for the program as a whole.

Basic operations with the CacheMap are along the lines of:

~~~{.cpp}
std::string SomeKey;//Generate a key unique to input

CacheData& MyCache=this->cache();//Get your cache

//Try to get the data, which is of type CoolDataType (ignore use_distcache for now)
std::shared_ptr<CoolDataType> Data=MyCache.get<CoolDataType>(SomeKey,use_distcache);

//One of two things happend
if(Data){//The cache had the data
   return *Data;//or do whatever it is you have to do with the data
}
else{//The cache did not have the data, generate it
   CoolDataType TheData;//Assume this is the generated data

   MyCache.set(MyHash,TheData,Policy);//Save it (ignore Policy for now)
   return TheData;//or again do whatever it is you have to do with the data
}
~~~

This is mirrored in Python as well:

~~~{.py}
SomeKey#Generate a key unique to input

MyCache=self.cache()#Get your cache

#Try to get the data (ignore use_distcache for now)
Data=MyCache.get(SomeKey,use_distcache)

#One of two things happend
if  Data: #The cache had the data
   return Data #or do whatever it is you have to do with the data
#The cache did not have the data, generate it
CoolDataType TheData() #Assume this is the generated data
MyCache.set(MyHash,TheData,Policy)#Save it (ignore Policy for now)
return TheData#or again do whatever it is you have to do with the data
}
~~~

Ultimately you can store whatever you want in the cache.  It is recommended that
you store the final result of your module for that input so you can
immediatly return it if called again. For most modules this is expected to be a
small amount of data (an energy, a new set of molecular orbital coeficients,
a rotation matrix, etc.).  If you expect your module's computation to take a
long time, or if there is a common intermediate for say a given molecule, it may
make sense to store that intermdiate as well.  This can be used to provide
primitive restart capabilities by allowing you to pick up on an interation
rather than having to start over.

TODO: Add documentation regarding data ownership and the use of pointers in
the cache.

Another unique (planned) aspect of the CacheMap class is its ability to move
data from RAM to disk depending on
resources.  Module writers are thus encouraged to not explicitly write anything
to disk, but rather let the CacheMap do it for you, if need be.  This avoids a
common problem in electronic structure packages where particularly for small
systems, an algorithm always writes what are presumed to be large intermediates
to disk, even if they actually fit in memory.  Hence you should not worry too
much about putting large tensors in the cache (although it may be prudent to
put them into the cache in blocks to aid in dumping to disk).

Indexing in the cache
---------------------

The cache is a map of keys to arbitrary data types. However, specifying
what the key is can be tricky. The key is a string that is meant to provide a
concise, unique
identification of exactly how this data was (or will be) generated.  Rather than
using a string like "Water hexamer with coordinates at..." what you will usually
want is a hash value of all variables that determine how to generate the piece
of data.  A hash is a string (usually comprised of the numbers 0 to 9 and the
letters a to f because it originates from hexadecimal numbers) that is generated
from a function called a hash function.  A hash function is a special function
that uniquely maps an object to a hash, such that two objects have the same hash
if and only if the objects are the same.  (Nearly) every class in the core of
Pulsar is capable of generating
a hash representation of itself (the standard name is `my_hash()`), so simply
concatenate the hashes to form the index.

One notable complication to this, however, is that many of the options in the
OptionMap may have no effect on the data [*e.g.*, printing, max number of
iterations (assuming the quantity converges before then), etc.]. Therefore, only
**significant** options should be hashed. These significant options take
the form of a set of strings which specify which option keys are to be
compared. The OptionMap has a function `hash_values` which takes a vector of
option keys, to hash, and returns a hash derived only from those keys.

CacheMap Internals and Policies
-------------------------------

In order to use the CacheMap effectively it helps to know a bit about the
internals.  The CacheMap is designed to work under distributed workflows (and
can be used to easily, but likely not effeciently, implement distributed
computing).  What this means is that there is a distinction between local and
global data (with the two being identical if Pulsar is run serially).  Data is
said to be local if it is available to the current process without the need for
an MPI call.  Global data starts as only local to the process that generated it,
but is additionally registered with a root process so that if another process
wants it, they can get it via MPI calls.  The MPI calls all happen transparently
to the user of the CacheMap class.

\note After obtaining data via an MPI call that data is still considered global
even though it is now local to your process.

The various policies are:

- NoPolicy: The data has none of the following policies
- CheckpointLocal: When a user calls `Checkpoint.checkpoint_local()` the data
  will be checkpointed
- CheckpointGlobal: When a user calls `Checkpoint.checkpoint_global()` the data
will be checkpointed
- DistributeGlobally: Data with this policy is registered with the root process
and made available to any process that wants it (will require MPI calls if the
data is not local to the asking process)

\note Policies are bit flags, what this means is you can set many policies at
once by using bit-wise or `|` .  To see if a policy is set you use bit-wise and
`&`.  Bit-wise operators are the same in Python and C++.

Checkpointing the Cache
-----------------------

The official cache is stored in the ModuleManager instance.  This can be
obtained from ModuleBase, but since checkpointing is more likely to occur in an
input file the ModuleManager instance is readily available.  To checkpoint
one first makes an instance of the Checkpoint class, which takes a shared
pointer to two CheckpointIO instances, one for the local backend and one for
the global backend.  At the moment you have two choices:

- DummyCheckpointIO: Does nothing
- BDBCheckpointIO: Uses Berkley DB to checkpoint.  An instance is made like:
  ~~~{.cpp}
  BDBCheckpointIO MyBackend("place/to/checkpoint");
  ~~~

Notes on file locations:
- The local checkpoint file is expected to be on a local file system, *i.e.* on
  the computer that the current process is running on
- The global checkpoint file is expected to be on a file system accessible to
  all processes (anywhere that is writable for a serial run or a shared file
  system for distributed runs).


The Checkpoint class is then made like:
~~~{.cpp}
Checkpoint mychk(std::make_shared<BDBCheckpointIO>("path/to/local/cache"),
                 std::make_shared<BDBCheckpointIO>("path/to/global/cache"));
~~~

and is the used to checkpoint like:

~~~{.cpp}
ModuleManager mm;
mychk.save_local_cache(mm);
mychk.save_global_cache(mm);
~~~

and reload a dump by:

~~~{.cpp}
ModuleManager mm;
mychk.load_local_cache(mm);
mychk.load_global_cache(mm);
~~~

This is also fully mirrored in Python:

~~~{.py}
mychk=psr.Checkpoint(psr.BDBCheckpointIO("local"),
                     psr.BDBCheckpointIO("global"))
mm=psr.ModuleManager()
mychk.save_local_cache(mm)
mychk.save_global_cache(mm)
mychk.load_local_cache(mm)
mychk.load_global_cache(mm)
~~~

## Deep-Dark Internals of the Cache (*i.e.* How it actually works)

The innerworkings of the cache rely on a mixture of templates and
serialization to deal with objects of arbitrary type.  When an object is
inserted into the cache it is converted into a `CacheMapEntry_` (which is just
a wrapper around a `GenericHolder<T>` instance and its associated policy).
`GenericHolder<T>` derives from `GenericBase`, which defines an interface that
will interact with the data in the holder in a serialized manner (*i.e.* if the
data is not serializable much of this interface is not available to it and
calling many of these functions will yield runtime errors).  When a user calls
`CacheMap::get` the `GenericBase` pointer is dynamically cast to
`GenericHolder<T>` if the cast succeeds the data is still in its input type
(*i.e.* it has not been serialized) and returns.  If the cast fails, the data is
presumed to be serialized and a dynamic cast to the serialized form is
attempted.  If that cast succeeds the data is deserialized and returned to the
user, if it fails you get a null pointer.

So that's all (relatively) straightforward the magic comes in with the
serialization.  There's two scenarios in which your data gets serialized.  The
first arises when your data is globally available, but not in your local cache.
In this case, the data is serialized so that it can be sent via MPI.  It then
arrives in your local cache serialized (and is only unserialized if you ask for
it to be).  The other scenario arises when data is checkpointed.  To checkpoint
data the data is serialized and dumped to disk, when the cache is reloaded the
serialized data is then stored in the cache in a serialized form (again it is
only unserialized with a `CacheMap::get` call).

As the last paragraph suggests serialization is the key to the two cool features
of Pulsar.  Both features will ignore non-serializable data (checkpointing will
not write it to disk and the distributed cache will not send it, forcing you to
recompute it).  Check out [this](@ref serialization) article on serialization
for more details, particularly as it relates to Pulsar.





