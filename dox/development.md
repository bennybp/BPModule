Using Pulsar in a Typical Development Workflow    {#devwork}
==============================================

One of the challanges to working with a project like Pulsar where the primary
API is in Python and the guts of the code are in C++ (a so-called embedded
system, specifically C++ embedded in Python) is being able to still use the
standard development tools (debugger, profiler, *etc.*).  The point of this
page is to help developers to use the tools they know and love with Pulsar.

IDE
---

This section includes tips for developing with integrated development
environments, or IDEs.  There are a variety of them and obviously we can't
cover them all, but hopefully you can extrapolate from the examples here to your
IDE of choice.  We have also included a little blurb about each IDE to highlight
their features for developers who may be interested in trying one out.

### Qt Creator

Qt creator (pronounced "cute" creator, BTW) is RMR's IDE of choice.  It is free
with the Qt library (which itself is free and opensource) and is one of the few
such IDEs that was developed for C++, rather than being developed for Java and
then backported to C++.  The result is, even on a laptop, Qt creator can handle
indexing of large C++ projects, with no serious lag.

TODO: write the rest.


Debugger
--------

By default CMake supports a build type `Debug` which will ensure that the
symbols remain in the compiled library.  This step is critical for debugging
C++ code.

### GDB
The GNU debugger, or gdb, is the *de facto* standard for debugging C++ code.
There are many excellent tutorials on the web for the command line interface.
Additionally, most IDEs support it in one form or another.  The main point to
take note when using gdb with Pulsar is that you'll need to give it Python as
the executable.  The result is a commandline like:

```{.sh}
gdb --args /path/to/python input_script_name
```
where `input_script_name` would be the Pulsar input script you are using.


Profiler
--------

### VTune

The key to using VTune with Pulsar is to ensure that the application being
profiled is actually Python.
