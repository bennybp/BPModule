import pulsar as psr
def load_ref_system():
    """ Returns diethylether as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      2.8816      0.0336      1.6217
        C      2.2891     -0.0221      0.7000
        H      2.6101      0.8010      0.0481
        H      2.5511     -0.9598      0.1926
        C      0.8085      0.0537      1.0218
        H      0.4993     -0.7786      1.6846
        H      0.5596      0.9983      1.5447
        O      0.1076     -0.0194     -0.2103
        C     -1.3023      0.0395     -0.0564
        H     -1.6493     -0.7929      0.5873
        H     -1.5903      0.9839      0.4464
        C     -1.9085     -0.0506     -1.4441
        H     -1.5780      0.7728     -2.0907
        H     -1.6404     -0.9880     -1.9487
        H     -3.0032     -0.0070     -1.3841
        """)
