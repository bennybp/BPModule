Serialization Within Pulsar                                     {#serialization}
===========================

Object-oriented languages like C++ and Python revolve around objects.  Under the
hood objects are complicated in that they are not just a series of bits, like
simple types such as integers and floats, but rather a mess of indirection to
give the developer an object-oriented coding interface.  Within C++ this
indirection amounts to pointers to member functions/data and virtual
tables.  Within Python the situation is even more complicated as there are
several layers of indirection on top of the actual pointers.  Nevertheless,
sometimes we need to treat a class as if it was just a series of bits (usually
when we want to store a class on disk for later use or when we want to transfer
it via say MPI).  This is when serialization comes in.  Serialization is the
process of mapping an object to a binary representation so that it can be
treated as if it was binary.

\note Before learning about serialization one of the first attempts at
"serializing" a class made by most programmers is to dump the class in a
JSON like format (some ASCII list format of the state of all member data).  This
is usually sufficient to recreate simple classes, but when the class contains
shared pointers or other levels of indirection it can quickly become too
complicated to dump all necessary information.

Practices for serializing Python and C++ objects differ between the two
languages.  Python provides "pickling" as a way of serializing Python objects.
C++ on the otherhand provides no such support out of the box; however, a
number of libraries have been designed for this purpose.  Pulsar currently has
adopted Cereal as our C++ serialization library.

## Serialization in C++

### Serializing Your Own Class (*i.e.* Invasive Serialization)
To aid in serializing a C++ class Pulsar defines the macro
`DECLARE_SERIALIZATION_FRIENDS` in `pulsar/util/Serialization.hpp`.  When you
put this macro within the body of your class (usually in the private section),
this macro will allow Cereal the access needed to serialize and
de-serialize an instance of your class.  Aside from the macro you need to define
a function `serialize` that takes a functor of arbitrary type (template the
function) and provides to it all of your class's member data.  A complete
example, for a trivial class, should make this more clear:

~~~.cpp
#include "pulsar/util/Serialization.hpp"

class MyFirstSerializableClass{
public:
    int x()const{return x;}

private:
    int x;//Some data that your class contains

    //The macro that allows Cereal access
    DECLARE_SERIALIZATION_FRIENDS

    //The function that registers your data
    template<typename Archive>
    void serialize(Archive & ar)
    {
        ar(x);
    }
};
~~~

Serialization is that easy.

\note Do not unwrap or unpackage your member data.  As long as the data is a
reference, pointer, or instance of a serializable type Cereal will do that for
you (and ensure reference counts etc. of shared pointers are correct).  If the
type of the data is not a serializable type recognized by Cereal you will have
to add to that class a `serialize` function analogous to the one you are
implementing.  If you do not have access to the class then you have to implement
serialization via...

### Serializing Someone Else's Class (*i.e.* Non-invasive Serialization)

When you can not add a macro and function to a class's definition it is still
possible to serialize the class (at least the publically accessible state of the
class).  It works via a free-function nearly identical to the private serialize
function.

~~~.cpp
template<typename Archive>
void serialize(Archive & archive,MyFirstSerializableClass & m)
{
  archive( m.x() );
}
~~~

### Serializing a Python Class

If you want to serialize a Python class for your own nefarious purposes, then by
all means go ahead and use pickle.  If you want your Python class to be
serializable within the guts of Pulsar that is automatic.  All
Python objects enter into the Pulsar C++ API as `pybind11::object` instances.
Pulsar already knows how to serialize these types of instances, so you're done!

## Serialization in More Detail

In an attempt to insulate the code from Cereal a bit, as well as to provide a
bit more functionality on top of Cereal.  To this end Pulsar defines the
`StdStreamArchive`.  This class defines serialization/deserialization guards, in
the form `begin_serialization`/`end_serialization` and `begin_unserialization`/
`end_serialization`, respectively.  These are to be placed around the actual
call to `serialize`/`deserialize` resepectively (is this somehow related to
thread safety?).  There exists two derived classes of this base class, one
for serializing to memory `MemoryArchive` and one for serializing to file
`FileArchive`.
