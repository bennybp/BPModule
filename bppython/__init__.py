import sys
import os
import importlib
from . import loader
from . import utils

bpcore = None

########################### 
# Initialize the path, etc
########################### 
# store the path, etc. We will reset later
modpath = "/home/ben/programming/BPModule/install/modules"
sys.path.insert(0, modpath)
sys.setdlopenflags(os.RTLD_NOW | os.RTLD_GLOBAL)

# Load the core module
bpcore = loader.LoadCoreModule("bpmodule_core")
print("Initialized core. bpcore = {}".format(bpcore))
