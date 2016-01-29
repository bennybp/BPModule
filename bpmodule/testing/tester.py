# For output
from bpmodule.output import Output, Error, Warning, Success, Debug
from bpmodule.util import Line

class Tester:
    def __init__(self, desc):
        self.nfailed = 0
        self.ntest = 1
        self.desc = desc


    def PrintHeader(self):
        Output(Line("="))
        Output("%1%\n", self.desc)
        Output(Line("="))



    def PrintResults(self):
        Output("\n\n")
        if self.nfailed > 0:
            Output(Line("="))
            Error("RESULTS: %1% failed\n", self.nfailed)
            Output(Line("="))
        else:
            Output(Line("="))
            Success("RESULTS: %1% failed\n", self.nfailed)
            Output(Line("="))
        Output("\n\n")


    def Test(self, desc, expected, func, *args):
        """
        Syntax: Test(desc, expected, func, ...)
        desc: The descrition of the test
        expected: either True or False depending on 
                  whether the function should pass
        func(...): The function to call, all values following the function
                   name will be forwarded to the function
        """
        failed = func(*args)
        
        fmt = "%|1$5| : %|2$-5|  %|3$-5|  %|4$-9|  : %5%\n"
        
        passed=(failed==expected)
        
        
        Output(fmt, self.ntest , expected , passed ,\
                "Success" if passed else "FAILED", desc)
        if not passed:
            self.nfailed+=1
        
        self.ntest += 1

       
         
