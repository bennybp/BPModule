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

Pulsar is a framework in the computer science sense of the word.  Meaning it is
an entire computational chemistry package, except for the computational
chemistry details.  What this means is it knows that there are approximate
methods for computing the energy of a chemical system, and what they look like
(they take a wavefunction and output an energy and a new wavefunction), but it
doesn't know how that method actually works.  This is where the highly-optimized
libraries come in.  Via a small wrapper-function (a goal for the community is to
come to a community consensus on what the interface should look like, and in
turn relegate the need for such a wrapper-function) the wavefunction object is
unpacked into a form the library understands, and then the result of the library
is translated into the return form Pulsar understands.  Pulsar simply calls this
wrapper-function when it needs the energy and never needs to know the details of
your method.

Pulsar Features
---------------

Pulsar's features nearly all stem from the fact that it looks functions up at
runtime.  The exact mechanism behind how Pulsar does this is described on
another page of this documentation (TODO: add reference), but for our present
purposes it suffices to say as long as your function has the expected signature
and Pulsar knows where your library resides, it can pull that function in at
runtime.

The big benefit of this comes from simplified builds.  Each function does not
have to link against every other function, nor does it have to link against the
Pulsar framework (you'll need the header files for the signature, but that's
it).
