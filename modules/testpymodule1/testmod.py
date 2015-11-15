import bpmodule as bp

class TestPyModule(bp.modulebase.Test_Base):
  def __init__(self, myid):
    super(TestPyModule, self).__init__(myid)


  def RunTest(self):
    bp.output.Output("+++ In TestPyModule: RunTest. Info: (%1%) %2% %3% v%4%\n", self.ID(), self.Key(), self.Name(), self.Version());
    bp.output.Output("   double_opt_def:    %1%\n", self.Options().Get("double_opt_def"));
    bp.output.Output("      int_opt_def:    %1%\n", self.Options().Get("int_opt_def"));
    bp.output.Output("     bool_opt_def:    %1%\n", self.Options().Get("bool_opt_def"));
    bp.output.Output("      str_opt_def:    %1%\n", self.Options().Get("str_opt_def"));
    bp.output.Output("\n");
    if self.Options().Has("double_opt"):
        bp.output.Output("       double_opt:    %1%\n", self.Options().Get("double_opt"));
    if self.Options().Has("int_opt"):
        bp.output.Output("          int_opt:    %1%\n", self.Options().Get("int_opt"));
    if self.Options().Has("bool_opt"):
        bp.output.Output("         bool_opt:    %1%\n", self.Options().Get("bool_opt"));
    if self.Options().Has("str_opt"):
        bp.output.Output("          str_opt:    %1%\n", self.Options().Get("str_opt"));

  def CallRunTest(self, s):
    bp.output.Output("+++ In TestPyModule: CallRunTest with %1%\n", s)

    tb = self.MLocator().GetModule_Test(s)
    bp.output.Output("  + Obtained scoped module ID %1%\n", tb.ID())
    tb.RunTest()
    bp.output.Output("  + Finished with scoped module %1%. Deleting automatically\n", tb.ID())

    bp.output.Output("+++Done\n");

  def Throw(self):
    bp.output.Warning("+++ In TestPyModule: Throwing an exception!\n")
    self.ThrowException("This is a test exception from python")

  def CallThrow(self, s):
    bp.output.Output("+++ In TestPyModule: CallRunTest with %1%\n", s)

    tb = self.MLocator().GetModule_Test(s)
    bp.output.Output("  + Obtained scoped module ID %1%\n", tb.ID())
    tb.Throw()

    # shouldn't be run?
    bp.output.Output("+++Done\n");


  def CalcTest(self, inputs):
    output = bp.datastore.CalcData()

    i = self.Options().Get("int_opt_def")
    d = self.Options().Get("double_opt_def")
    d2 = d * i;

    if inputs.Has("TEST_INPUT_1"):
        d2 *= inputs.GetCopy("TEST_INPUT_1")

    output.Set("TEST_RESULT_1", d2)
    return output



def CreateModule(name, myid):
    return TestPyModule(myid)

