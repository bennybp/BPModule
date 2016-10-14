Testing                                                               {#testing}
=======

In order to ensure that Pulsar is working well it is imparative to test it.
These tests also allow us to determine if a check-in breaks something.  This is
only possible if we have unit tests.  We also need what are called acceptance
tests, which ensure that the interface our users see is also working.  Testing
Pulsar is a little bit different than testing other libraries because the core
of Pulsar is only loadable at runtime, you can't link against it (for more on
this see [module vs. shared library](@ref shared).


## Setting Up Tests and Using the Pulsar Tester Class

All of Pulsar's tests should run through CMake's CTest module.  To facilitate
this we have defined the following CMake macros that you can use to register
your tests:

- `pulsar_py_test(name)` : This adds a test for the Python interface of a class
  The test should be in a file named `name.py`
- `pulsar_cpp_test(name)` : This adds a test for the C++ interface of a class.
  The actual test should be in a file `name.cpp`. 
- `pulsar_test(name)` : This adds a test called name that is contained in a
   Python file called `name.py`

Testing the C++ interface of a class requires a bit of explanation.  Pulsar
itself is a dynamically loadable library and must be "dlopened".  To make this
easier we have created a simple Python script `TestCpp.py` that will start a
module manager and load the test as a module.  Within this module is where the
tests need to run.

For all tests one should use the Pulsar testing Tester.  This is an object
designed to simplify and unify the testing procedure.  The Tester has both a
C++ and a Python interface.




Basic Python usage is very similar except that we can't overload functions
(going through `Pybind11` we can, but that doesn't work with templates, which
is the current case).

~~~{.py}
from TestFxns.py import * #Gives us the Tester class and sets up Pulsar

#Make a tester instance
my_tester=Tester("Description of the tests to run")

#Test that A_fxn returns true when given the arguments 1,2, and 3
my_tester.test("A_fxn(1,2,3)",true,true,A_fxn,1,2,3)

#Test that A_fxn throws when given the arguments 1,2, and 4
my_tester.test("A_fxn(1,2,4)",false,true,A_fxn,1,2,4)

#Test that two values are equal
my_tester.test_value("Is 3==3?",3,3)

#Test that two floats are equal to within 0.0001 (this is the default and can be
#omitted)
my_tester.test_Value("Is 2.123==2.123",2.123,2.123,0.0001)

#Print the result will also throw if any test failed
my_tester.print_result()
~~~


## Unit Testing

Unit testing is a method of testing that ensures the smallest possible parts of
the code work.  For our purposes a unit test is a test of a class or a series of
related functions.  

Unit tests should:
- Be for a specific class
  - Each class should have its own unit test and be tested individually
- All functions should be tested including constructors/destructors
- All possible throws should be checked
  - For example: if a function is supposed to throw if the input is less than 0
    test it once with an input less than 0
- All languages that a class are accessible from should be tested
  - The different language tests should be to the extent possible identical
    - Ensures classes work identically from all languages
  - Again, due to the somewhat unique setup of Pulsar this is largely only
    applicable to the Pulsar Module interfaces (derive a energy method and test
    it for example from both C++ and Python)
  - Classes like System will only be tested from Python
- Try to minimize includes/imports in a test to better ensure you are testing
  what you think you are testing

A basic Python unit test template looks like:
~~~{.py}
#Bare bones sets up Pulsar using staged version so test can occur in build dir 
from .TestFxns import *

#Make a Tester instance used throughout this test
tester = psr.testing.Tester("Describe the series of tests here")

#Run tests here

#Call to finalize the testing procedure
tester.print_results()

#Shuts down Pulsar gently to avoid seg-faults from leaving libraries open
psr.finalize()
~~~

