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

## Printing Tables

Many results gathered in electronic structure theory garner themselves
to being printed in tables, but making a nice looking table can be
a hassle owing to the amount of formatting required.  The series of
classes in this header file are designed to fix that.

As of right now this class should be able to support any rectangular
table and any data that can be boost::lexical_cast to a string.
The number of columns and rows is arbitrary.  You can pick from
three alignments within a cell (left, center, right).  The border of
each cell is customizable, as is the border of the table.  By being
creative within the row class it should be possible to emulate the
Latex multicolumn command and fathomably the multirow command.  Not
sure what other options one wants.

\section sec2 History

It's always good to keep track of what you did and why it didn't work

From the get go the table class has always been designed to be general
in that it could print arbitrary data, in arbitrary layouts.  Arbitrary
data suggests the use of templates; however, this incarnation avoids this
as will be seen.  Anyways, the first try was C++98 compliant and relied
on template recursion to print the table.  It had two serious problems:
1) The table was limited to 10 columns (which isn't much when you are
printing say a 4 index quantity and a couple values relating to it) and
the data had to be copied into the table, sometimes with a transpose.
My second table class used variadic templates (hence only C++11 compliant)
and added the concept of a TableColumn class.  The variadic template
was nice because we were no longer bound to 10 columns, and the
TableColumn class was nice because it avoided the transpose problems in
a creative way.

My next thought was that in the end everything that goes into the
table needs to be printed, this suggests that rather than having to
deal with a templated Table class maybe I can deal with lexical casts
of arbitrary data.  The results are all of type std::string and hence
I don't have to worry about templates. I then wanted to add options for
making Row classes or Column classes; however the logic of what trumps
what got too complicated (if cell 2,3 is specified in both a row and
a column who prints cell 2,3?, how do I know the cell is empty)

The Lexical cast is a great idea, but the two separate classes are too
much.  What if I just emulate Excel?  The base class is broken into
cells.  The table keeps track of the character between columns and
rows.  We then provide fill column and fill row options, as
well as equivalent options for alignment and borders.  This seems
to work well and is the current implementation.

#### Usage

So how do you use the table class?  Pretty simple.  For concreteness,
assume we have the following data:
\code
double DataPoints[]={1.0, 1.1, 1.2,
                     2.0, 2.1, 2.2,
                     3.0, 3.1, 3.2};
std::string ColTitles[]={"Col1","Col2","Col3"};
std::string RowTitles[]={"Row1","Row2","Row3"};
\endcode

This data is naturally arranged into a table that is 4 columns
wide and 4 rows long (a row for the column titles and a column
for the row titles).  We declare our table and fill it:
\code
//Makes a 4x4 table
Table OurTable(4,4);

//Add the column titles which go from column 1 to 3
OurTable.AddRow(ColTitles,1,3);

//Add the row titles which go from row 1 to 3
OurTable.AddCol(RowTitles,1,3);

//Add the data
for(i=0;i<3;i++)
   OurTable.AddCol(&DataPoints[i],//Get a pointer to our 1st element
                   1,3,//puts it in rows 1 to 3
                   3);//Stride between adjacent data points

//To print the table to standard out
std::cout<<OurTable;

\endcode

The above code is fine for a quick and dirty table, but often you
want more pizazz.  For that reason you can add vertical and
horizontal delimiters to your table as well:

\code
//If we wanted a row of '*' above and below the table:
//Above row 0
OurTable.SetHBorder(0,'*');
//Above row 4 (i.e. under row 3)
OurTable.SetHBorder(4,'*');

//If we wanted a column of '|' between column 1 and 2:
//To the left of column 1
OurTable.SetVBorder(1,'|');

//Say we wanted column 1 to be left aligned
OurTable.SetAlign(LEFT);
\endcode

For even finer control I allow you to manually tweak any border,
width, or alignment, but be warned I haven't tested the
following well, but they operate under the same principles
as the examples above so I anticipate it working as advertised:
\code

//Make cell (0,1) have a border of "|"
//First add a space so that the columns still line up
SetVBorder(1,' ');
//Now change cell (0,1)'s border
GetVBorder(0,1)='|';


//Right align cell (2,3)
GetCell(2,3).SetAlign(RIGHT);

//Change its width
GetCell(2,3).SetWidth(5);
\endcode

Using the last two code sections one should be able to set
all of the parameters to their liking.  Pay particular attention
to the first example shown in the code above as this is a gothca
if you don't want borders on every cell in the column, but you
still want the cells to line up.
 
 