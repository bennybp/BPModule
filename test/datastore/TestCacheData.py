import pulsar as psr

def run_test():
    tester=psr.PyTester("Testing CacheData Python Bindings")

    return tester.nfailed()
