import bppython as bp

class TestPyModule_Option(bp.bpcore.Test_Base):
  def __init__(self, myid, mstore, minfo):
    super(TestPyModule_Option, self).__init__(myid, mstore, minfo)


  def RunTest(self):
    bp.Output("+++ In TestPyModule_Option. This is not meant to be run\n")

  def CallRunTest(self, s):
    bp.Output("+++ In TestPyModule_Option. This is not meant to be run\n")

  def Throw(self):
    bp.Output("+++ In TestPyModule_Option. This is not meant to be run\n")

  def CallThrow(self, s):
    bp.Output("+++ In TestPyModule_Option. This is not meant to be run\n")





def CreateModule(name, myid, mstore, minfo):
    # Not really meant to be run
    return TestPyModule_Option(myid, mstore, minfo)

