How to Print                                                        {#printing}
============

Despite it being one of the first things people learn in a language, printing is
 actually quite complicated.  The actual guts of what goes on below the call 
involves many interactions of the language and the operating system.  Futhermore,
printing becomes tricky when you want to redirect it, run using process-based
parallelism (think MPI), or synchronize the printing of many independent
 objects.  Pulsar strives to do this for you an a semi-automatic way, but you 
have to meet it half way.

## Printing From Within a Module

If you are within a module, the module manager has hooked you up with an object
called *out*, which is an instance of an OutputStream.  It supports several cool
syntaxes:

~~~{.cpp}
   //C++ stream syntax works just fine
   out<<"This is valid printing"<<std::endl;

   //Any C++ object that overloads the << operator for std::ostreams can be used
   //For example, the pulsar System class does
   out<<mol<<std::endl;

   //You can also do printf like syntax
   out.output("A printf like string %d\n",1);

  //The printf like syntax also works for errors
  out.error("This is an error\n")

  //warnings
  out.warnings("This is a warning\n");

  //debug printing
  out.debug("This is debug text\n");
~~~

The *out* object is also exported to Python so the printf-like syntaxes will 
work there too with literally the same syntax (well I guess you dont need the
semicolon and the comments need to be pound signs, but you get the idea...).

## Printing Via Free Functions
As part of the Pulsar core you will find the Output library.  Contained within 
this library are some wrappers that allow you to do the same sorts of printing 
to the current output buffer.  The syntax is similar:

~~~{.cpp}
   #include <pulsar/output/GlobalOutput.hpp>

   //General printing
   pulsar::output::print_global_output("String to print, in printf like syntax");

   //General error
   pulsar::output::print_global_error("An error");

   //General warning
   pulsar::output::print_global_warning("A warning");

  //General debug
  pulsar::output::print_global_debug("A debug warning");
~~~

Again, this is mirrored in Python.  The relevant syntaxes are:

~~~{.py}
   #General printing
   pulsar.output.print_global_output("String to print, in printf like syntax")

   #General error
   pulsar.output.print_global_error("An error")

   #General warning
   pulsar.output.print_global_warning("A warning")

   #General debug
   pulsar.output.print_global_debug("A debug warning")
~~~

## Obtaining a Printing Object Outside a Module

Sometimes you want to print in a function that is not part of a module and so
does not have an *out* member.  Because printing involves calls to the operating
system it is somewhat global in nature and it is not unreasonable to want to
print to the same stream from many different places.  To this end Pulsar
provides free functions that will return the current streams as follows:

~~~{.cpp}
#include<pulsar/output/GlobalOutput.hpp>

pulsar::output::OutputStream& out=pulsar::output::get_global_output();

//out can now be used as above
out<<"Printing to the standard output"<<std::endl;
~~~

This functionality is also available in Python:

~~~{.py}
out=pulsar.output.get_global_output()

out.output("This is getting printed")
~~~