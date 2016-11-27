import pulsar as psr

class TestPyModule1(psr.TestModule):
  def __init__(self, myid):
    super(TestPyModule1, self).__init__(myid)

  def run_test_(self):
    self.out.output("Inside TestPyModule1: id = {}\n".format(self.id()))


class TestPyModule2(psr.TestModule):
  def __init__(self, myid):
    super(TestPyModule2, self).__init__(myid)

  def run_test_(self):
    self.out.output("Inside TestPyModule2: id = {}\n".format(self.id()))
    exstr = "TestPyModule2 throwing exception"
    ex = psr.PulsarException(exstr)
    ex.append_info("moduleid", self.id());
    raise ex;


class TestPyModule3(psr.TestModule):
  def __init__(self, myid):
    super(TestPyModule3, self).__init__(myid)

  def run_test_(self):
    self.out.output("Inside TestCppModule3: id = {}\n".format(self.id()))
    mkey = self.options().get("OTHER_MODULE")
    self.out.output("Creating module OTHER_MODULE = {}\n".format(mkey))
    m = self.create_child(mkey);
    self.out.output("Running {}\n".format(mkey))
    m.run_test()

