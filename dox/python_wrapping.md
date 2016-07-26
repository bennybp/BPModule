#Notes and Guidelines for the Python/C++ Interface
This document is meant to help future coders with exporting/importing their classes across the Python/C++ interface.  At the moment I only have experience with porting C++ to Python so vice versa will need to be written at a later time.  This document assumes you know the difference between pointers, references, smart pointers, etc. and are using PyBind11 to facilitate the interface.  It also assumes your goal is to maintain the same object behavior on both sides of the interface.


##Memory Management
Python and C++ manage memory in two very different ways so this is probably the trickiest part of the interface.  C++ is very explicit with how an object is stored Python is not.  At the interface it is very important to know how owns what memory.
Pybind11 has made the conventions that if you return a:

- pointer, Python owns the memory
- rvalue, it is moved to Python (and Python owns the memory)
- lvalue, Python gets a copy (and now owns the copies memory)

If these policies don't work for you you can tell Pybind11 what to do instead (see their advanced topics tab, specifically return value policies).
