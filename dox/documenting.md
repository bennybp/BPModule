Documenting Pulsar                                                {#documenting}
==================
This page describes documentation philosophy in Pulsar as well as conventions
one should use while documenting features of the core library.

## Documenation Program Decisions

Pulsar's main documentation uses `Doxygen`.  `Doxygen` is the *de facto* standard 
for
documenting C++ code.  Unfortunately, `Doxygen` does not play well with Python in
the sense that it doesn't parse doc strings (it does suppport writing your
documenation in Python comments, but that is not usual Python practice).  The
de facto standard for Python is `Sphinx`, which is abismal for documenting C++.  
Of the two languages, C++ is more involved and `Doxygen` natively knows all of 
its
quirks and syntax rules (even the more obscure ones).  Getting Sphinx to this
level would require you to basically write a compiler (on some level `Doxygen` 
is
a symbolic compiler).  Furthermore `Doxygen` is far more automatic than `Sphinx` 
in
the sense that it keeps a "symbol table" so that it can cross link classes,
members, etc. making very tightly coupled documentation.  

`Doxygen` supports plugins and the `Doxypypy` is just that.  `Doxypypy` adds to
`Doxygen` the ability to parse doc strings that are written meeting one of 
several
standards.  To obtain `Doxypypy` you can go to their github page or run:
~~~{.sh}
pip install doxypypy
~~~
Once you have Doxypypy, running:
~~~{.sh}
doxygen
~~~
in the Pulsar root directory should be sufficient to generate Python API
documentation from the doc strings of the Python files (and also gives you the
C++ API documentation and user's/developer's manual).

\note I am in no stretch of the imagination an expert in either documentation
program (less so with `Sphinx`).  The above comments reflect my experience with
these programs and there may be ways around the shortcomings listed above that I
could not find with Google searching.

## Documenting C++

Basically follow normal Doxygen standards.  At the moment we suggest the
following conventions to help keep the documentation concise:

- Use `\copydoc function_name` when possible to avoid duplicating documentation
- Prefer after member documentation for very short documentation, e.g.
   ~~~{.cpp}
   int natoms_;///<The number of atoms
   ~~~

## Documenting Python
Documentation in Python goes in doc strings.  For example:
~~~{.py}
class MyClass:
    """This is the doc string for the class
       
       There's more on this line.
    """
    
    def __init__(self):
        """This is the doc string for the __init__ function
 
           I am now going to explain how this class is constructed in
           excruciating detail.
        """
~~~

There are a host of ways to document Python code.  Pulsar follows the Google
Standard, which is surprisingly difficult to find given who makes it... To aid in
that effort I've reproduced the key parts here.

Basically the doc string is seperated into several parts:

- Brief description 
  - Starts on the same line as the first triple quote
  - May continue onto other lines
  - Ends with a puncuation mark
  - Separated from remainder of comment with a blank line
  - If the brief is sufficient to document the entire function it terminates 
    with the end triple quote, e.g.
    \code{.py}
    def my_fxn():
        """Returns the number 5."""
        return 5
    \endcode
- Long description
  - Follows the brief description and may be paragraphs long
  - Free form environment for the most part
  - Is terminated by one of the following sections or (if no such sections are
    applicable) the end triple quote
- Attributes
  - For classes you list the attributes after the long description like:
       \code{.py}
       class My_Class:
           """Brief.

           Long.
           Longer.
           Longest.
           
           Attributes:
              my_attribute_1: description of this attribute including the type
              my_attribute_2: repeat the above
           """
       \endcode
- Arguments
  - For functions the arguments to the function follow the long description
  - Same syntax as attributes, just replace `Attributes:` with `Args:`
- Returns
  - For functions returning something it should be documented in a section
    titled `Returns:` like:
    \code{.py}
    def my_fxn(arg1):
        """This function is stupid.
  
        It does nearly nothing, but yet I am writing all this documentation.

        Args:
            arg1: A string, but it is completely ignored 

        Returns:
            A message informing you how stupid this function is
        """
        return "This function is so stupid it thought a quarterback was a refund."
    \endcode

- Raises
  - If your function raises any exceptions they should be documented in this
    section, which should be titled `Raises:`
- Yields
  - Replaces `Returns:` for generators
- You may use Doxygen commands such as \verbatim\f$\beta\f$\endverbatim to 
  generate the Greek letter beta

## Adding Pages

Rather than clutter the code with tons of examples of how to use it.  We
recommend making a page for code that is likely to see lots of use and deserves
a tutorial on its own.  For example we have the 
[Making and Using Systems](@ref systems) page to describe making and using a
System.  This avoids 100s of extra lines in the actual `System.hpp` file.  If
you want to add a page:
- Place the actual file in the `dox` folder
   - The page should be written using markdown
   - The extension should be `.md`
   - You can also use Doxygen commands in it (and indeed sometimes you'll need
     to as Doxygen sometimes stutters with markdown)
- Add your file to `dox/developers.doxcfg` so Doxygen knows to use it
- Add a link to your documentation in `mainpage.dox` (in an appropriate section)





