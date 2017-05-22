Overview of the Pulsar Project                                       {#overview}
==============================

This page is meant to provide an overview of the Pulsar computational chemistry
framework, or Pulsar for short.

Computational Chemistry in the Dark Ages (*i.e.* before Pulsar)
---------------------------------------------------------------

In the days of yore most computational chemistry development occured in one of
several monolithic packages.  Each of these packages was originally created to
address a particular aspect such as molecular mechanics, multireference
electronic structure theory, or coupled cluster theory.  These packages were
for the most part developed without recourse to what we now know as modern
computer science techniques.  The result is rather than making a series of
stand-alone libraries these packages tend to be highly-coupled.  A direct result
of this is that there is much common code among these packages that has been
reimplemented in each package simply because it is easier to rewrite it than it
is to try to incorporate a library.

Funding agencies are trying to put a stop to this.  In particular, they are
growing tired of funding each package to do the same thing simply because it
is to hard to use another package's implementation.  The result is many groups
have taken to creating stand-alone, highly-optimized libraries for many of the
facets of computational chemistry.  Nevertheless the situation has not changed
within thes monolithic packages and it becomes increasingly hard to support
these libraries (or fully incoroporate them).  This is where Pulsar comes in.

What is Pulsar?
---------------

Pulsar is a framework in the computer science sense of the word.  Meaning it is
an entire computational chemistry package, except for the computational
chemistry part.  More specifically, Pulsar has ideas about many computational
chemistry concepts, such as what it means to compute the energy of a system,
what it means to compute a shell quartet, *etc.* but it doesn't know how to
actually do any of that.  Instead, it allows you to tell it to call your
function when Pulsar needs an energy or an integral . At first this may be a bit
hard to grasp, so let's consider what the typical call stack looks like.

\image html images/pulsar_idea.png

As Pulsar runs it comes to a point where it needs, say, the energy of a chemical
system.  Not knowing any computational chemistry, it calls your function,
which has some agreed upon input API.  Your method computes the energy and then
returns the energy to Pulsar using the return API.  Superficially this looks
very similar to a normal call stack, but what makes Pulsar special is that the
binding of your function to the input and output APIs happens at runtime.  In
turn, Pulsar can call your function, or really any function satisfying those
APIs, without having to be compiled against it.  This allows you to change out
the guts of an algorithm on-the-fly.




