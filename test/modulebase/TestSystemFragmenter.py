import pulsar as psr



class TestFragger(psr.SystemFragmenter):
    def __init__(self,id):
        super(TestFragger,self).__init__(id)

    def fragmentize_(self,sys):
        nsi=psr.NMerInfo()
        nsi.sn={0,}
        nsi.nmer=sys
        nsi.weight=1.0
        return {"0 ":nsi}

def run_test():
    tester=psr.PyTester("Testing SystemFragmenter Python Bindings")
    mm=psr.ModuleManager()
    mm.load_lambda_module(TestFragger,"TestFragger","Test_fragger")
    fragger=mm.get_module("Test_fragger",0)
    sys=psr.make_system("""
    H 0.0 0.0 0.0
    H 0.0 0.0 0.89
    """)
    corr_answer=psr.NMerInfo()
    corr_answer.sn={0,}
    corr_answer.nmer=sys
    corr_answer.weight=1.0
    print(fragger.fragmentize(sys))
    tester.test_return("Can call fragmentize",True,{"0 ":corr_answer},
        fragger.fragmentize,sys)
    tester.print_results()
    return tester.nfailed()
