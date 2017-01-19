import pulsar as psr

class MyPyBase(psr.ModuleBase):
    def __init__(self,myid):
        super(MyPyBase,self).__init__(myid,"MyPyBase")


def run_test():
    tester=psr.PyTester("Testing ModuleCreationFuncs Python Bindings")

    mcf=psr.ModuleCreationFuncs()
    module_type,not_type="my module type","not a type"
    tester.test_call("Can add Python module",True,mcf.add_py_creator,module_type,MyPyBase);
    tester.test_return("Python module is registered",True,True,mcf.has_creator,module_type)
    tester.test_return("Handles non-registered type",True,False,mcf.has_creator,not_type)

    tester.print_results()
    return tester.nfailed()
