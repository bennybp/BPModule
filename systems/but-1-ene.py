import pulsar as psr
def load_ref_system():
    """ Returns but-1-ene as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      1.6729     -0.4054     -0.4971
        C      0.6079      0.5240      0.0441
        C     -0.6123     -0.2271      0.4475
        C     -1.8376      0.1200      0.0724
        H     -0.4435     -1.1003      1.0895
        H      1.9576     -1.1697      0.2382
        H      2.5821      0.1472     -0.7659
        H      1.3297     -0.9314     -1.3972
        H      0.9905      1.0696      0.9302
        H     -2.0461      0.9732     -0.5675
        H     -2.7219     -0.4283      0.3819
        H      0.3667      1.3031     -0.7065
        """)
