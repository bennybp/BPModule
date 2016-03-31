# For output
from bpmodule.output import GlobalOutput, GlobalError, GlobalSuccess, GlobalDebug
from bpmodule.util import Line

class Tester:
    def __init__(self, desc):
        self.nfailed = 0
        self.ntest = 1
        self.desc = desc


    def PrintHeader(self):
        GlobalOutput(Line("="))
        GlobalOutput("{}\n".format(self.desc))
        GlobalOutput(Line("="))



    def PrintResults(self):
        GlobalOutput("\n\n")
        if self.nfailed > 0:
            GlobalOutput(Line("="))
            GlobalError("RESULTS: {} failed\n".format(self.nfailed))
            GlobalOutput(Line("="))
        else:
            GlobalOutput(Line("="))
            GlobalSuccess("RESULTS: {} failed\n".format(self.nfailed))
            GlobalOutput(Line("="))
        GlobalOutput("\n\n")


    def Test(self, desc, expected, func, *args):
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
            GlobalDebug("Exception: {}\n".format(str(e)))
            success = False
        
        fmt = "{:5} : {:<5}  {:<5}  {:<9}   : {}\n"
        
        passed=(success==expected)
        
        
        GlobalOutput(fmt.format(self.ntest , expected , success , "Success" if passed else "FAILED", desc))
        if not passed:
            self.nfailed+=1
        
        self.ntest += 1

    def TestValue(self, desc, v1, v2):
        self.Test(desc, True, lambda w1, w2: w1 == w2, v1, v2)
         
