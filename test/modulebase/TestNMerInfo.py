import pulsar as psr

def run_test():
    tester=psr.PyTester("Testing NMerInfo Python Bindings")
    MyU=psr.AtomSetUniverse()
    H,H1=psr.create_atom([0.0,0.0,0.0],1),psr.create_atom([0.0,0.0,0.89],1)
    MyU.insert(H)
    MyU.insert(H1)
    H2=psr.System(MyU,True)
    SN={1,2,3}

    NI1,NI3=psr.NMerInfo(),psr.NMerInfo()
    NI1.sn=SN
    NI1.nmer=H2
    NI1.weight=2.0

    tester.test_equal("Serial number set",NI1.sn,SN)
    tester.test_equal("System is set",NI1.get_nmer(),H2)
    tester.test_double("Weight is set",NI1.weight,2.0)
    NI2=psr.NMerInfo(NI1)
    tester.test_equal("Serial number is copied",NI2.sn,SN)
    tester.test_equal("System is copied",NI2.get_nmer(),H2)
    tester.test_double("Weight is copied",NI2.weight,2.0)
    tester.test_equal("Equality works",NI1,NI2)
    tester.test_not_equal("Inequlaity works",NI1,NI3)

    tester.print_results()
    return tester.nfailed()
