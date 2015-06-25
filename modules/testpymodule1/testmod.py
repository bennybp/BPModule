import bppython as bp

class TestPyModule(bp.bpcore.Test_Base):
  def __init__(self, myid, mstore, options):
    super(TestPyModule, self).__init__(myid, mstore, options)


  def RunTest(self):
    bp.Output("+++ Inside TestPyModule: RunTest\n")

  def RunCallTest(self, s):
    bp.Output("+++ Inside TestPyModule: RunCallTest with %1%\n", s)

    tb = self.MStore().GetScopedModule_Test(s)
    bp.Output("  + Obtained scoped module ID %1%\n", tb.ID())
    tb.RunTest()
    bp.Output("  + Finished with scoped module %1%. Deleting automatically\n", tb.ID())

    tb2 = self.MStore().GetModule_Test(s)
    bp.Output("  + Obtained module ID %1%\n", tb2.ID())
    tb2.RunTest()
    bp.Output("  + Finished with module %1%. Deleting\n", tb2.ID())
    self.MStore().Delete(tb2.ID())

    bp.Output("+++Done\n");


def CreateModule(name, myid, mstore, options):
    return TestPyModule(myid, mstore, options)
  


