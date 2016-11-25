from pulsar import ModuleCreationFuncs
from .TestPyModules import *

def insert_supermodule():
    cf = ModuleCreationFuncs()
    cf.add_py_creator("TestPyModule1", TestPyModule1)
    return cf
