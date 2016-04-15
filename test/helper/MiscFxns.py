import os
import sys

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "../", "modules")
sys.path.insert(0, bppath)


import bpmodule as bp

def ApplyBasis(syst,bsname,bslabel="primary",bstype=bp.system.ShellType.Gaussian):
    return bp.system.ApplySingleBasis(bstype,bslabel,bsname,syst)
