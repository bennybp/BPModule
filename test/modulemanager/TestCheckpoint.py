import pulsar as psr

was_called=False
H=psr.create_atom([0.0,0.0,0.0],1)
class MyPyBase(psr.TestModule):
    def __init__(self,myid):
        super(MyPyBase,self).__init__(myid)
    def run_test_(self):
        policy=psr.CacheData.CachePolicy.CheckpointLocal
        global was_called
        for data in [("Some data",2.1),("Other data",H)]:
            cache_entry=self.cache().get(data[0],False)
            if cache_entry:
                if not was_called:
                    raise psr.PulsarException("Data shouldn't be there")
                elif not cache_entry==data[1]:
                    cache_entry.print(psr.get_global_output())
                    data[1].print(psr.get_global_output())
                    raise psr.PulsarException("Data is not cached value")
            elif was_called:
                raise psr.PulsarException("Data should be here")
            self.cache().set(data[0],data[1],policy)
        was_called=True

def run_test():
    tester=psr.PyTester("Testing Checkpoint Python Bindings")
    for i in range(2):
        mychk=psr.Checkpoint(psr.BDBCheckpointIO("local"),
                             psr.BDBCheckpointIO("global"))
        mm=psr.ModuleManager()
        mm.load_lambda_module(MyPyBase,"MyPyBase","Module")
        if i==1:mychk.load_local_cache(mm)
        my_mod=mm.get_module("Module",0)
        msg="Checkpointing data" if i==0 else "Uncheckpointing data"
        tester.test_call(msg,True,my_mod.run_test)
        if i==0:mychk.save_local_cache(mm)
    tester.print_results()
    return tester.nfailed()
