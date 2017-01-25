import pulsar as psr
import os

class MyPyBase(psr.TestModule):
    def __init__(self,myid):
        super(MyPyBase,self).__init__(myid)
    def run_test_(self):
        return

def run_test():
    tester=psr.PyTester("Testing ModuleManager Python Bindings")
    mm=psr.ModuleManager()
    minfo=psr.ModuleInfo()
    mod_name,not_name,opt_key="my module","Not a module","Option1"
    opt=2.0

    minfo.options.add_option(opt_key,psr.OptionType.Float,True,None,"",None)
    load_module=mm.load_module_from_minfo
    tester.test_return("has non-existent module",True,False,mm.has_key,mod_name)
    tester.test_call("No path causes failure",False,load_module,minfo,mod_name)
    minfo.path="a/fake/path"
    tester.test_call("No handler",False,load_module,minfo,mod_name)
    minfo.type="c_module"
    tester.test_call("Not a path",False,load_module,minfo,mod_name)
    cwd=os.path.dirname(os.path.realpath(__file__))
    minfo.path=os.path.join(cwd,"CXXModule.so")
    tester.test_call("No name",False,load_module,minfo,mod_name)
    minfo.name="C++ Module"
    tester.test_call("load_module",True,load_module,minfo,mod_name)
    tester.test_return("has module",True,True,mm.has_key,mod_name)
    tester.test_return("size",True,1,mm.size)
    tester.test_return("module key info",True,minfo,mm.module_key_info,mod_name)
    tester.test_call("module key info non-existent key",False,
        mm.module_key_info,not_name)
    tester.test_call("test all no required opt",False,mm.test_all)
    tester.test_call("set required opt bad key",False,mm.change_option,not_name,
        opt_key,opt)
    tester.test_call("set required opt bad opt key",False,mm.change_option,
        mod_name,not_name,opt)
    tester.test_call("get non-existent module",False,mm.get_module,not_name,0)
    tester.test_call("get bad parent ID",False,mm.get_module,mod_name,9)
    tester.test_call("get not required options ",False,mm.get_module,mod_name,0)
    tester.test_call("set required opt",True,mm.change_option,mod_name,opt_key,
        opt)
    tester.test_call("get module",True,mm.get_module,mod_name,0)
    tester.test_call("can't change option for used key",False,mm.change_option,
        mod_name,opt_key,opt)
    tester.test_call("can't duplicate to existing key",False,mm.duplicate_key,
        not_name,mod_name)
    tester.test_call("can't duplicate non-existing key",False,mm.duplicate_key,
        not_name,not_name)
    new_name="new mod name"
    tester.test_call("duplicate key",True,mm.duplicate_key,"my module",new_name)
    unq_key=mm.generate_unique_key()
    tester.test_return("unique key",True,False,mm.has_key,unq_key)
    tester.test_call("add lambda module",True,mm.load_lambda_module,MyPyBase,"Py Module",unq_key)
    tester.test_call("can get module from creation funcs",True,mm.get_module,
        unq_key,0)
    tester.print_results()
    return tester.nfailed()
