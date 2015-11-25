import bpmodule as bp

class TestOptions(bp.modulebase.Test_Base):
  def __init__(self, myid):
    super(TestOptions, self).__init__(myid)


  def RunTest(self):
    bp.output.Output("+++ In TestOptions. This is not meant to be run\n")

  def CallRunTest(self, s):
    bp.output.Output("+++ In TestOptions. This is not meant to be run\n")

  def Throw(self):
    bp.output.Output("+++ In TestOptions. This is not meant to be run\n")

  def CallThrow(self, s):
    bp.output.Output("+++ In TestOptions. This is not meant to be run\n")

