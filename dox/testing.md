Testing                                                               {#testing}
=======

In order to ensure that Pulsar is working well it is imparative to test it.
These tests also allow us to determine if a check-in breaks something.  This is
only possible if we have unit tests.  We also need what are called acceptance
tests, which ensure that the interface our users see is also working.

## Setting Up Tests

All of Pulsar's tests should run through CMake's CTest module.  To facilitate
this there is a series of macros: pulsar_cpp_test, pulsar_py_test, and
pulsar_test.  Respectively, these define CTest tests for a C++ executable, a
python module, and both  These macros take a single argument which is the name
of the test and it is expected that the .cpp and .py functions also have this
name.

For all tests one should use the Pulsar testing tester.  This is an object
designed to simplify and unify the testing procedure.  Basic usage is like:
~~~{.cpp}

pulsar::testing::Tester my_tester("Description of the tests to run");

~~~


## Unit Testing

Unit testing is a method of testing that ensures the smallest possible parts of
the code work.  For our purposes a unit test is a test of a class.  Each class
should have its own unit test and the unit test should ensure that each function
is working correctly.  Note that this means you also have to check errors, for
example if a function is supposed to throw if the input is less than 0, there
should be a test to ensure this throw occurs.  Unit tests should check the
native language of a class as well as all other languages for which there are
bindings (i.e. if its a C++ class exported to Python make sure you check both
C++ bindings and Python bindings).
