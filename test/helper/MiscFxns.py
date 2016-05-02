import os
import sys

# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "../", "modules")
sys.path.insert(0, psrpath)


import pulsar as psr

def ApplyBasis(syst,bsname,bslabel="primary"):
    return psr.system.ApplySingleBasis(bslabel,bsname,syst)
