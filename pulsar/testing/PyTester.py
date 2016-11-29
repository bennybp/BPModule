from .. import *

class PyTester(TesterBase):
    """Adds duck-typed test function and gets around no overloads."""

    def __init__(self, desc):
        super(PyTester, self).__init__(desc)

    def test_return(self, desc, should_pass, expected, func, *args):
        try:
            r = func(*args)
            success = (expected == r) and should_pass
        except Exception as ex:
            print_global_debug("Caught exception in test\n"); 
            print_global_debug(str(ex) + "\n"); 
            success = not should_pass
        self.test(desc,success)

    def test_call(self, desc, should_pass, func, *args):
        try:
            func(*args)
            success = should_pass
        except Exception as ex:
            print_global_debug("Caught exception in test\n"); 
            print_global_debug(str(ex) + "\n"); 
            success = not should_pass

        self.test(desc,success)


    def test_equal(self, desc, v1, v2):
        self.test(desc, v1 == v2)

    def test_not_equal(self, desc, v1, v2):
        self.test(desc, (v1 != v2))


    def test_double(self,desc,v1,v2,tol=0.0001):
        """Same as Tester's test with two values."""
        if type(v1)==float and type(v2)==float:
           self.test_float(desc,v1,v2,tol)
        else:
            self.test(desc,v1==v2)



##################
# For testing on the python side
##################
def py_test_function(func, *args):
    try:
       func(*args)
    except Exception as e:
        print_global_debug(str(e) + "\n")
        return 0
    except:
        return 0

    return 1



def py_test_bool_function(func, *args):
    try:
        return func(*args)

    except Exception as e:
        print_global_debug(str(e) + "\n")
        return 0

    except:
        return 0    
         
