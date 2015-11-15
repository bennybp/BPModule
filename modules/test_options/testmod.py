import bpmodule as bp

class TestPyModule_Option(bp.modulebase.Test_Base):
  def __init__(self, myid):
    super(TestPyModule_Option, self).__init__(myid, mlocator, minfo)


  def RunTest(self):
    bp.output.Output("+++ In TestPyModule_Option. This is not meant to be run\n")

  def CallRunTest(self, s):
    bp.output.Output("+++ In TestPyModule_Option. This is not meant to be run\n")

  def Throw(self):
    bp.output.Output("+++ In TestPyModule_Option. This is not meant to be run\n")

  def CallThrow(self, s):
    bp.output.Output("+++ In TestPyModule_Option. This is not meant to be run\n")





def CreateModule(name, myid):
    # Not really meant to be run
    return TestPyModule_Option(myid)

