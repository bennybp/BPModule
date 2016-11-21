from . import TestPyModules
from pulsar.modulemanager import ModuleCreationFuncs

def insert_supermodule():
    cf = ModuleCreationFuncs()
    cf.add_py_creator("TestPyModule1", TestPyModule1.TestPyModule1)
    return cf
