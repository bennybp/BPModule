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
    exstr = "TestPyModule2 throwing exception"
    ex = psr.PulsarException(exstr)
    ex.append_info("moduleid", self.id());
    raise ex;


