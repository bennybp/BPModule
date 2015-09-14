from .modinfo import *

# SO file
from .testing import *


# For output
from bpmodule import output




def PrintHeader(s):
    output.Output(output.Line("="))
    output.Output("%1%\n", s)
    output.Output(output.Line("="))



def PrintResults(nfailed):
    output.Output("\n\n")
    if nfailed > 0:
        output.Output(output.Line("="))
        output.Error("RESULTS: %1% failed\n", nfailed)
        output.Output(output.Line("="))
    else:
        output.Output(output.Line("="))
        output.Success("RESULTS: %1% failed\n", nfailed)
        output.Output(output.Line("="))
    output.Output("\n\n")
