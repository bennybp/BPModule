from pulsar import ModuleCreationFuncs
from .TestPyModules import *

def insert_supermodule():
    cf = ModuleCreationFuncs()
    cf.add_py_creator("TestPyModule1", TestPyModule1)
    cf.add_py_creator("TestPyModule2", TestPyModule2)
    return cf
