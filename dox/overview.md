Overview of the Pulsar Project                                       {#overview}
==============================

This page is meant to provide an overview of the Pulsar computational chemistry
framework, or Pulsar for short.

Pulsar Overview
---------------

Pulsar is designed to be a flexible framework for developing and running
computational chemistry calculations. This flexibility is mainly achieved
by run-time loading of fine-grained modules, where each module is designed
to perform only a small part of a computational chemistry calculation.
These modules are developed separately from the core and from each other,
and are expected to obey a well-defined interface.

This separation has some benefits for both users and developers. For users,
this allows for composing different types of calculations that would be hard
to specify in a input-file format. Pulsar also has powerful checkpointing and
output features.

For developers, fine-grained modularity allows developers to focus only
on the module you are developing, leaving the rest of the calculation up
to the other modules. It also simplifies building, since the module is compiled
independently. Pulsar also handles some desired features that are often overlooked
or tedious to implement, such as checkpointing.

Lastly, it allows module developers to be able to distribute their code
separate from the core and separate from other modules.

To be fair, there are also drawbacks to such an approach. It requires that
the community develop the module API such that it covers most or all use cases;
It may be difficult to hammer the requirements for new modules into an existing
interface.  Also, as a concern to core developers, the core API must
be kept stable, as changes to the core would require changes to all modules.
Many small changes to the API would place place a maintenance burden on research
groups.
