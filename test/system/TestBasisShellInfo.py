from TestFxns import *

tester = Tester("Testing the BasisShellInfo class")
BSI=psr.system.BasisShellInfo
cGTO = psr.system.ShellType.CartesianGaussian

alpha=[3.42525091, 0.62391373, 0.16885540]
c=[0.15432897, 0.53532814, 0.44463454]
alpha2=[]

threw = False
try:
    s6=BSI(cGTO, 0, 3, 1, alpha2, c)
except Exception as e:
    threw = True
tester.test_value("BSI : Constructor check passed", True, threw)

s1=BSI(cGTO, 0, 3, 1)
s2=BSI(cGTO, 0, 3, 1, alpha, c)
tester.test("BSI : Inequality works", True, True, s1.__ne__,s2)
s3=BSI(s2)
tester.test_value("BSI : Copy constructor and copying data works", s2, s3)

tester.print_results()