#Notes and Guidelines for the Python/C++ Interface
This document is meant to help future coders with exporting/importing their classes across the Python/C++ interface.  At the moment I only have experience with porting C++ to Python so vice versa will need to be written at a later time.  This document assumes you know the difference between pointers, references, smart pointers, etc. and are using PyBind11 to facilitate the interface.


##Memory Management
Python and C++ manage memory in two very different ways so this is probably the trickiest part of the interface.  C++ is very explicit with how an object is stored Python is not.


