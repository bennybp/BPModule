Testing                                                               {#testing}
=======

\note This page covers testing protocols for testing Pulsar and Pulsar's
standard modules.  Ultimately, testing modules falls to module developers and
although we encourage them to follow similar testing procedures they are in no
way obligated to.

In order to ensure that Pulsar is working well it is imparative to test it.
These tests also allow us to determine if a check-in breaks something.  This is
only possible if we have unit tests.  We also need what are called acceptance
tests, which ensure that the interface our users see is also working.  Finally,
we have stress tests, which aim to determine how well our classes hold up under
large amounts of data or many calls.  Testing
Pulsar is a little bit different than testing other libraries because the core
of Pulsar is only loadable at runtime, you can't link against it (for more on
this see [module vs. shared library](@ref shared)).


## Setting Up Tests and Using the Pulsar Tester Class

All of Pulsar's tests should run through CMake's CTest module.  To facilitate
this we have defined the following CMake macros that you can use to register
your tests:

- `pulsar_py_test(folder name)` : This adds a test for the Python interface of a class
  The test should be in a file named `name.py`
- `pulsar_cpp_test(folder name)` : This adds a test for the C++ interface of a class.
  The actual test should be in a file `name.cpp`. 
- `pulsar_test(folder name)` : This adds a test called name that is contained in a
   Python file called `name.py`

Where `folder` is the directory within the `test_stage` directory that the
test will be included under.

For all tests one should use the Pulsar testing Tester.  This is an object
designed to simplify and unify the testing procedure.  The Tester has both a
C++ and a Python interface accessed throug CppTester and PyTester respectively.

When designing and debugging tests to use the `ctest` command it is useful to
keep the following command in mind:

~~~{.cmake}
ctest -R<test_name> -V
~~~

which will run all tests that have `test_name` somewhere in the name and will
output the output to the screen as well as the file
`<build_directory>/Testing/Temporary/LastTest.log`

### Testing in C++

Testing the C++ interface of a class requires a bit of explanation.  Pulsar
itself is a dynamically loadable library and must be "dlopen-ed".  To make this
easier we have created a macro to wrap C++ tests.

A basic C++ test setup will look like this:
~~~{.cpp}
#include <pulsar/class/I/am/testing>
#include <pulsar/testing/CppTester.hpp>

TEST_SIMPLE(TheNameOfMyTest){

    //Make a Tester instance
    CppTester my_tester("Description of the tests to run");

    //Test that A_fxn returns true when given the arguments 1,2, and 3
    my_tester.test_return("A_fxn(1,2,3)",true,true,A_fxn,1,2,3);

    //Test that A_fxn throws when given the arguments 1,2, and 4
    my_tester.test_return("A_fxn(1,2,4)",false,true,A_fxn,1,2,4);

    //Test that two values are equal
    my_tester.test_equal("Is 3==3?",3,3);

    //Test that two floats are equal to within 0.0001 (this is the default 
    //tolerance and can be omitted)
    my_tester.test_double("Is 2.123==2.123",2.123,2.123,0.0001);

    //Test a member function
    MyClass A;
    tester.test_member_return("Is 2.0==A.value(1)?",true,2.0,&MyClass::value,&A,1);

    //Test a void function
    tester.test("testing void",true,void_fxn,param1);

    //Test a void member function
    tester.test_member_call("A.void_fxn(1) works",true,&MyClass::void_fxn,&A,1);


    //Print the result
    my_tester.print_result();
    return tester.nfailed();

}//End test
~~~

### Testing Python

Testing Python side is nearly identical except that we
don't have to distinguish between member and non-meber
functions
~~~{.py}
import pulsar as psr

def run_test(): #All Python tests should be in a function with this name (similar to macro idea from C++)
    #Make a tester instance
    my_tester=psr.PyTester("Description of the tests to run")

    #Test that A_fxn returns true when given the arguments 1,2, and 3
    my_tester.test_return("A_fxn(1,2,3)",true,true,A_fxn,1,2,3)

    #Test that A_fxn throws when given the arguments 1,2, and 4
    my_tester.test_return("A_fxn(1,2,4)",false,true,A_fxn,1,2,4)

    #Test that two values are equal
    my_tester.test_equal("Is 3==3?",3,3)

    #Test that two floats are equal to within 0.0001 (this is the default and can be
    #omitted)
    my_tester.test_double("Is 2.123==2.123",2.123,2.123,0.0001)

    #Print the result
    my_tester.print_result()
    return my_tester.nfailed()
~~~

## Unit Testing

Unit testing is a method of testing that ensures the smallest possible parts of
the code work.  For our purposes a unit test is a test of a class or a series of
related functions.  

Unit tests should:
- Be for a specific class
  - Each class should have its own unit test and be tested individually
- All functions should be tested including constructors
- All possible throws should be checked
  - For example: if a function is supposed to throw if the input is less than 0
    test it once with an input less than 0
    - The Tester class will catch the throw for you and will mark the test a
      success assuming you told it the function was supposed to fail
- All languages that a class are accessible from should be tested
  - The different language tests should be, to the extent possible, identical
    - Ensures classes work identically from all languages
    - Even with the macros you'll need to cast to a const version of your
      object to test the const versions of functions
- Try to minimize includes/imports in a test to better ensure you are testing
  what you think you are testing


## Acceptance Testing

Acceptance testing focuses on testing common code interactions that we expect
our users to do.  These should be tests that do not reproduce the unit tests.
So even though we expect users to create wavefunctions often, creating a
wavefunction, in all supported ways, is covered in the wavefunction unit test.

## Stress Testing

\todo Document Pulsar stress testing practices
