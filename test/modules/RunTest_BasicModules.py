import pulsar as psr

def run_test():

    tester = psr.PyTester("Basic Module Loading/Calling")
    with psr.ModuleAdministrator() as ma:
        psr.print_global_output(str(ma) + "\n")

    tester.print_results()
    return tester.nfailed()
