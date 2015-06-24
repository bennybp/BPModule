import bppython as bp

class TestPyModule(bp.bpcore.Test_Base):
  def __init__(self, myid, mstore, options):
    super(TestPyModule, self).__init__(myid, mstore, options)


  def RunTest(self):
    print("Running test from PyModule")



def CreateModule(name, myid, mstore, options):
    return TestPyModule(myid, mstore, options)
  


