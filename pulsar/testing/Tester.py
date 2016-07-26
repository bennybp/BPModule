# For output
from pulsar.output import *
from pulsar.util import line

class Tester:
    def __init__(self, desc):
        self.nfailed = 0
        self.ntest = 1
        self.desc = desc


    def print_header(self):
        print_global_output(line("="))
        print_global_output("{}\n".format(self.desc))
        print_global_output(line("="))



    def print_results(self):
        print_global_output("\n\n")
        if self.nfailed > 0:
            print_global_output(line("="))
            print_global_error("RESULTS: {} failed\n".format(self.nfailed))
            print_global_output(line("="))
        else:
            print_global_output(line("="))
            print_global_success("RESULTS: {} failed\n".format(self.nfailed))
            print_global_output(line("="))
        print_global_output("\n\n")


    def test(self, desc, expected, func, *args):
        """
        Syntax: Test(desc, expected, func, ...)
        desc: The description of the test
        expected: True if the function should pass, False if it should fail
        func(...): The function to call, all values following the function
                   name will be forwarded to the function
        """

        try:
            success = func(*args)
        except Exception as e:
            print_global_debug("Exception: {}\n".format(str(e)))
            success = False
        
        fmt = "{:5} : {:<5}  {:<5}  {:<9}   : {}\n"
        
        passed=(success==expected)
        
        
        print_global_output(fmt.format(self.ntest , expected , success , "Success" if passed else "FAILED", desc))
        if not passed:
            self.nfailed+=1
        
        self.ntest += 1

    def test_value(self, desc, v1, v2, Tol=0.00001):
        if type(v1)==float or type(v2)==float:
            self.test(desc,True,lambda w1, w2: abs(w1-w2)<Tol,v1,v2)
        else:
           self.test(desc, True, lambda w1, w2: w1 == w2, v1, v2)
    
         
