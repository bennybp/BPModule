Testing                                                               {#testing}
=======

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

- `pulsar_py_test(name)` : This adds a test for the Python interface of a class
  The test should be in a file named `name.py`
- `pulsar_cpp_test(name)` : This adds a test for the C++ interface of a class.
  The actual test should be in a file `name.cpp`. 
- `pulsar_test(name)` : This adds a test called name that is contained in a
   Python file called `name.py`

For all tests one should use the Pulsar testing Tester.  This is an object
designed to simplify and unify the testing procedure.  The Tester has both a
C++ and a Python interface.

### Testing in C++

Testing the C++ interface of a class requires a bit of explanation.  Pulsar
itself is a dynamically loadable library and must be "dlopen-ed".  To make this
easier we have created a simple Python script `TestCpp.py` that will start a
module manager and load the test as a module.  Within this module is where the
tests need to run.  You will then have to define this module and compile it.
Thankfully, there's a macro `TEST_CLASS` that does all this for you.

A basic C++ test setup will look like this:
~~~{.cpp}
#include <pulsar/class/I/am/testing>
#include "TestCXX.hpp"//Loads the Tester object and defines the TEST_CLASS macro

TEST_CLASS(TheNameOfMyTest){

    //Make a Tester instance
    Tester my_tester("Description of the tests to run");

    //Test that A_fxn returns true when given the arguments 1,2, and 3
    my_tester.test("A_fxn(1,2,3)",true,true,A_fxn,1,2,3);

    //Test that A_fxn throws when given the arguments 1,2, and 4
    my_tester.test("A_fxn(1,2,4)",false,true,A_fxn,1,2,4);

    //Test that two values are equal
    my_tester.test("Is 3==3?",3,3);

    //Test that two floats are equal to within 0.0001 (this is the default 
    //tolerance and can be omitted)
    my_tester.test("Is 2.123==2.123",2.123,2.123,0.0001);

    //Print the result will also throw if any test failed
    my_tester.print_result();

}//End test
~~~

Here's a couple of hints for C++ testing:
- Call your Tester instance "tester" so you can use the macros
  - Could pass the tester in, but then most tests don't fit within 80 characters
- To test a member function use `std::bind` it looks something like:
    ~~~{.cpp}
    MyClass A;
    //Makes a functor that calls A.fxn() 
    auto Afxn=std::bind(&MyClass::fxn,A,std::placeholder::_1,std::placeholder::_2);
    
    //Pass the functor to the test method
    my_tester.test("Test A.fxn that returns 1 when given 2",true,1,Afxn,2);
    ~~~
  - You need to include a placeholder for each argument that will be passed to
    your function (including the MyClass instance)
  - Testing overloaded methods is possible by casting the function pointer
    ~~~{.cpp}
    MyClass A;

    //Assume A.fxn() returns an int 1 when given either 2 or 2.0, thus
    //fxn has two overloads: one for int and one for double
    //This is the double overload's type
    using DoubleOvl_t=int(MyClass::*)(double);
    
    //This is then a functor to it
    auto doubleovl=std::bind(static_cast<DoubleOvl_t>(&MyClass::fxn),
        std::placeholder::_1,std::placeholder::_2);

    //And finally the test
    my_tester.test("Testing double overload",true,1,doubleovl,A,2.0);
    ~~~
    - This is the "proper" way to do this and super annoying, I have defined a
      macro `TEST_FXN` that takes the test description, true/false if it should
      pass, the expected value, the function, and assumes your Tester instance
      is called "tester"
      ~~~{.cpp}
      TEST_FXN("This Fxn works",true,3.14,FxnThatMakesPi(3,5,9));
      ~~~
  - The Tester needs some work if it's going to work with constructors or 
    functions without returns
    - In the meantime I have defined a macro `TEST_VOID` that takes the test
      description, true/false if it should pass, the function, and assumes your
      Tester instance is called "tester"
      ~~~{.cpp}
      TEST_VOID("This function works",true,FxnWithNoReturn(Arg1,Arg2),tester);
      ~~~

### Testing Python

Testing Python side is much easier because we don't have to worry about the
types of the function; however, we can't overload functions so the
 Tester's methods change name.  Aside from that not much else has changed:

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
my_tester.test_value("Is 2.123==2.123",2.123,2.123,0.0001)

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

\todo Go back and use macros in old C++ tests for readability

## Acceptance Testing

Acceptance testing focuses on testing common code interactions that we expect
our users to do.  These should be tests that do not reproduce the unit tests.
So even though we expect users to create wavefunctions often, creating a
wavefunction, in all supported ways, is covered in the wavefunction unit test.

## Stress Testing

\todo Document Pulsar stress testing practices
