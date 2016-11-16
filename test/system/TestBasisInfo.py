from TestFxns import *

tester = Tester("Testing the BasisShellInfo class")
cGTO = psr.ShellType.CartesianGaussian
alpha=[3.42525091, 0.62391373, 0.16885540]
c=[0.15432897, 0.53532814, 0.44463454]

FakeD=psr.BasisShellInfo(cGTO, 2, 3, 1, alpha, c)
BI=psr.BasisInfo()
BI.description="My Description"
BI.shells=[FakeD]
shells=[FakeD]
BI2=psr.BasisInfo(BI)
tester.test_value("Description is set","My Description",BI.description)
tester.test_value("Shells are set",shells,BI2.shells)
tester.test_value("Copy constructor works",BI,BI2)
tester.test("Inequality works",True,False,BI.__ne__,BI2)

tester.print_results()

