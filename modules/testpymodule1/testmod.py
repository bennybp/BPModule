import bppython as bp

class TestPyModule(bp.bpcore.Test_Base):
  def __init__(self, myid, mstore, minfo):
    super(TestPyModule, self).__init__(myid, mstore, minfo)


  def RunTest(self):
    bp.Output("+++ In TestPyModule: RunTest. Info: (%1%) %2% %3% v%4%\n", self.ID(), self.Key(), self.Name(), self.Version());
    bp.Output("   double_opt_def:    %1%\n", self.GetOption("double_opt_def"));
    bp.Output("      int_opt_def:    %1%\n", self.GetOption("int_opt_def"));
    bp.Output("     bool_opt_def:    %1%\n", self.GetOption("bool_opt_def"));
    bp.Output("      str_opt_def:    %1%\n", self.GetOption("str_opt_def"));
    bp.Output("\n");
    if self.HasOption("double_opt"):
        bp.Output("       double_opt:    %1%\n", self.GetOption("double_opt"));
    if self.HasOption("int_opt"):
        bp.Output("          int_opt:    %1%\n", self.GetOption("int_opt"));
    if self.HasOption("bool_opt"):
        bp.Output("         bool_opt:    %1%\n", self.GetOption("bool_opt"));
    if self.HasOption("str_opt"):
        bp.Output("          str_opt:    %1%\n", self.GetOption("str_opt"));

  def CallRunTest(self, s):
    bp.Output("+++ In TestPyModule: CallRunTest with %1%\n", s)

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

  def Throw(self):
    bp.Warning("+++ In TestPyModule: Throwing an exception!\n")
    self.ThrowException("This is a test exception from python")

  def CallThrow(self, s):
    bp.Output("+++ In TestPyModule: CallRunTest with %1%\n", s)

    tb = self.MStore().GetScopedModule_Test(s)
    bp.Output("  + Obtained scoped module ID %1%\n", tb.ID())
    tb.Throw()

    # shouldn't be run?
    bp.Output("+++Done\n");


  def CalcTest(self, inputs):
    output = bp.bpcore.CalcData()

    i = self.GetOption("int_opt_def")
    d = self.GetOption("double_opt_def")
    d2 = d * i;

    if inputs.Has("TEST_INPUT_1"):
        d2 *= inputs.GetCopy("TEST_INPUT_1")

    output.Add("TEST_RESULT_1", d2)
    return output



def CreateModule(name, myid, mstore, minfo):
    return TestPyModule(myid, mstore, minfo)

