from .modinfo import *

# SO file
from .testing import *


# For output
from bpmodule.output import Output, Error, Warning, Success, Debug, Line


##################
# For testing on the python side
##################
def PyTestFunc(itest, desc, expected, func, *args):
    fmt = "%|1$5| : %|2$-5|  %|3$-5|  %|4$-9|  : %5%\n"

    try:
       func(*args)
    except Exception as e:
        if expected == False:
            Output(fmt, itest, "False", "False", "Success", desc)
            return 0
        else:
            Debug(str(e) + "\n")
            Error(fmt, itest, "True", "False", "FAILED", desc)
            return 1
    except:
        if expected == False:
            Output(fmt, itest, "False", "False", "Success", desc)
            return 0
        else:
            Error(fmt, itest, "True", "False", "FAILED", desc)
            return 1

    if expected == True:
        Output(fmt, itest, "True", "True", "Success", desc)
        return 0
    else:
        Error(fmt, itest, "False", "True", "FAILED", desc)
        return 1



def PyTestBoolFunc(itest, desc, expected, func, *args):
    fmt = "%|1$5| : %|2$-5|  %|3$-5|  %|4$-9|  : %5%\n"

    try:
        res = func(*args)
        if res == True:
            if expected == True:
                Output(fmt, itest, "True", "True", "Success", desc)
                return 0
            else:
                Error(fmt, itest, "False", "True", "FAILED", desc)
                return 1
        else:
            if expected == False:
                Output(fmt, itest, "False", "False", "Success", desc)
                return 0
            else:
                Error(fmt, itest, "True", "False", "FAILED", desc)
                return 1

    except Exception as e:
        if expected == False:
            Output(fmt, itest, "False", "False", "Success", desc)
            return 0
        else:
            Debug(str(e) + "\n")
            Error(fmt, itest, "True", "False", "FAILED", desc)
            return 1
    except:
        if expected == False:
            Output(fmt, itest, "False", "False", "Success", desc)
            return 0
        else:
            Error(fmt, itest, "True", "False", "FAILED", desc)
            return 1




def PrintHeader(s):
    Output(Line("="))
    Output("%1%\n", s)
    Output(Line("="))



def PrintResults(nfailed):
    Output("\n\n")
    if nfailed > 0:
        Output(Line("="))
        Error("RESULTS: %1% failed\n", nfailed)
        Output(Line("="))
    else:
        Output(Line("="))
        Success("RESULTS: %1% failed\n", nfailed)
        Output(Line("="))
    Output("\n\n")
