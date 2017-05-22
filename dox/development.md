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

#### Setting Qt Creator up

From the file menu select new file or project.  In the resulting pop-up window
select import project in the left pane and import existing project in the center
pane.

\image html images/qt_setup_1.png "Step 1: Import project."

Give the project a name and point Qt to the root directory of Pulsar-Core.

\image html images/qt_setup_2.png "Step 2: Set directory."

Select the files to import into Qt Creator.  By default Qt only chooses the
files typically associated with C++.  To get all files you'll have to check the
top most box (or check all the sub-boxes manually).

\image html images/qt_setup_3.png "Step 3: Select files."

Finally, Qt creator will ask to put some of its configuration files under git
control.  Please select no version control to avoid pushing them to the
repository (the information within in them is good only on your system).

\image html images/qt_setup_4.png "Step 4: Qt creator configuration files."

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
