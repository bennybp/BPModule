Terminology                                                       {#terminology}
===========

The following is a list of words and phrases used within
Pulsar and its documentation. In particular, some words are borrowed
from computer science, but (as usual) given a slightly different connotation.

Much of the documentation of Pulsar uses a language we have agreed upon to
describe key concepts. These terms should hopefully be self explanatory without
this page, but for reference, they are included here.

- <b>Framework</b> The scaffolding on which a computer program is built. There
  are various definitions/arguments of framework and how frameworks differ from
  libraries. We subscribe to the definition in which a framework is code
  into which pieces are inserted and then called in an organized manner.
  We call the pulsar core is the framework.

- <b>Module</b> A module is code that is written to perform a specific task
  in computational chemistry. Modules are dynamically inserted into the
  \b framework. The framework then calls the modules for tasks that need
  to be completed. In Pulsar, the modules would be thought of as libraries.

- <b>Module Base</b> An abstraction of the module for what task it is to perform.
  Modules must conform to the base's API and implement the required functionality. 

- <b>Super Module</b> A collection of modules that are related in some fashion.
  For example, all the modules may be written by the same group and have the same external
  dependencies (such as a math library).

- <b>Module Administrator</b> An object that handles loading, creating, and
  other related functions of modules. This also includes checkpointing and
  reading from checkpoint files. The module manager is a very powerful and central
  object in Pulsar. In Pulsar, the \ref pulsar::ModuleManager is typically what is used by
  modules themselves (to call other modules, in particular),
  while the input uses the ModuleAdministrator (in Python)

- <b>Module Key</b> An arbitrary string representing a module. This is generally chosen
  by the end user, either directly or indirectly. Inside Pulsar, they are meaningless, but
  allow the user to specify the relationship between modules and to specify which module
  a particular part of the calculation should use.

- <b>Loading a Module</b> Inserting the module into the module manager, so that it
  can be used in the calculation.

- <b>Creating a Module</b> Actually instantiating a module so that it can be used.
