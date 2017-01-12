import pulsar as psr
import sys
import os
sys.path.append(os.path.dirname(__file__))
from testing_datastore import *

def run_test():
    tester=psr.PyTester("Testing CacheData Python Bindings")
    cd1=get_cachedata()
    v1,v2=[1.0,2.0,3.0],[2.0,3.0,4.0]
    policy=psr.CacheData.CachePolicy.NoPolicy
    key,not_key="Vector 1","Not a Key"
    cd1.set(key,v1,policy)
    keys={"my_module%%"+key}
    tester.test_return("size",True,1,cd1.size)
    tester.test_return("get_keys",True,keys,cd1.get_keys)
    tester.test_return("get valid key works",True,v1,cd1.get,key,False)
    tester.test_return("get invalid key works",True,None,cd1.get,not_key,False)
    cd1.set(key,v2,policy)
    tester.test_return("set overwrites",True,v2,cd1.get,key,False)
    tester.test_call("erase",True,cd1.erase,key)
    tester.test_return("erase works",True,0,cd1.size)
    tester.print_results()
    return tester.nfailed()
