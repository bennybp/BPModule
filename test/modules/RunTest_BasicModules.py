import os
import sys
import pulsar as psr

# So we can find "testmodules"
thispath = os.path.dirname(os.path.realpath(__file__))
modpath = os.path.dirname(thispath)
sys.path.insert(0, modpath)
print(thispath)

def run_test():

    tester = psr.PyTester("Basic Module Loading/Calling")
    with psr.ModuleAdministrator() as ma:
        psr.print_global_output(str(ma) + "\n")

        ma.load_module("testmodules", "TestCppModule1", "TESTMOD1")
        ma.load_module("testmodules", "TestPyModule1", "TESTPYMOD1")

        m1 = ma.get_module("TESTMOD1", 0)
        m2 = ma.get_module("TESTPYMOD1", 0)

        m1.run_test()
        m2.run_test()

        del m1
        del m2

    tester.print_results()
    return tester.nfailed()
