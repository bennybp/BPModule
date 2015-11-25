from . import TestPyModule1
from . import TestOptions

def CreateModule(name, myid):
    if name == "TestPyModule1":
      return TestPyModule1.TestPyModule1(myid)
    elif name == "TestOptions":
      return TestOptions.TestOptions(myid)
    elif name == "TestOptions_int":
      return TestOptions.TestOptions(myid)
    elif name == "TestOptions_float":
      return TestOptions.TestOptions(myid)
    elif name == "TestOptions_bool":
      return TestOptions.TestOptions(myid)
    elif name == "TestOptions_str":
      return TestOptions.TestOptions(myid)
    elif name == "TestOptions_listint":
      return TestOptions.TestOptions(myid)
    elif name == "TestOptions_listfloat":
      return TestOptions.TestOptions(myid)
    elif name == "TestOptions_listbool":
      return TestOptions.TestOptions(myid)
    elif name == "TestOptions_liststr":
      return TestOptions.TestOptions(myid)
    else:
      raise RuntimeError("TODO - FIXME")
