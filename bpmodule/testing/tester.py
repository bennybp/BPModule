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
        desc: The descrition of the test
        expected: either True or False depending on 
                  whether the function should pass
        func(...): The function to call, all values following the function
                   name will be forwarded to the function
        """
        failed = func(*args)
        
        fmt = "{:5} : {:<5}  {:<5}  {:<9}   : {}\n"
        
        passed=(failed==expected)
        
        
        GlobalOutput(fmt.format(self.ntest , expected , passed , "Success" if passed else "FAILED", desc))
        if not passed:
            self.nfailed+=1
        
        self.ntest += 1

       
         
