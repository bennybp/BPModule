import bppython as bp

class TestPyModule(bp.bpcore.Test_Base):
  def __init__(self, myid, mstore, options):
    super(TestPyModule, self).__init__(myid, mstore, options)


  def RunTest(self):
    print("+++ Inside TestPyModule: RunTest")

  def RunCallTest(self, s):
    print("+++ Inside TestPyModule: RunCallTest with {}".format(s))
    tb = self.MStore().GetModule_Test(s)
    print("  + Obtained module ID {}".format(tb.ID()))
    tb.RunTest()
    print("  + Finished with {}. Deleting".format(tb.ID()))
    self.MStore().Delete(tb.ID())



def CreateModule(name, myid, mstore, options):
    return TestPyModule(myid, mstore, options)
  


