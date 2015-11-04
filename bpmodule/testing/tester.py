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
        failed = func(*args)

        fmt = "%|1$5| : %|2$-5|  %|3$-5|  %|4$-9|  : %5%\n"

        if failed == 0:
            if expected == True:
                Output(fmt, self.ntest, "True", "True", "Success", desc)
            else:
                Error(fmt, self.ntest, "False", "True", "FAILED", desc)
                self.nfailed += 1 
        else:
            if expected == False:
                Output(fmt, self.ntest, "False", "False", "Success", desc)
            else:
                Error(fmt, self.ntest, "True", "False", "FAILED", desc)
                self.nfailed += 1 

        self.ntest += 1
       
         
