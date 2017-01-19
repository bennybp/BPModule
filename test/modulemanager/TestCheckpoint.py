import pulsar as psr

was_called=False

class MyPyBase(psr.TestModule):
    def __init__(self,myid):
        super(MyPyBase,self).__init__(myid)
    def run_test_(self):
        data=self.cache().get("Some data",False)
        global was_called
        if data:
            if not was_called:
                raise psr.PulsarException("Data shouldn't be there")
            elif not data==2.1:
                raise psr.PulsarException("Data is not cached value")
        elif was_called:
            raise psr.PulsarException("Data should be here")
        self.cache().set("Some data",2.1,
            psr.CacheData.CachePolicy.CheckpointLocal)
        was_called=True

def run_test():
    tester=psr.PyTester("Testing Checkpoint Python Bindings")
    for i in range(2):
        mychk=psr.Checkpoint(psr.BDBCheckpointIO("local"),
                             psr.BDBCheckpointIO("global"))
        mm=psr.ModuleManager()
        minfo=psr.ModuleInfo()
        minfo.name="MyPyBase"
        mcf=psr.ModuleCreationFuncs()
        mcf.add_py_creator(minfo.name,MyPyBase)
        mm.load_module_from_mcf(minfo,"Module",mcf)
        if i==1:mychk.load_local_cache(mm)
        my_mod=mm.get_module("Module",0)
        tester.test_call("call "+str(i),True,my_mod.run_test)
        if i==0:mychk.save_local_cache(mm)
    tester.print_results()
    return tester.nfailed()
