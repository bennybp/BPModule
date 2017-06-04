import pulsar as psr
def load_ref_system():
    """ Returns propane as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      1.9131     -0.1215      1.0479
        C      1.2683     -0.0636      0.1619
        H      1.5702      0.8259     -0.4068
        H      1.4890     -0.9392     -0.4630
        C     -0.1934     -0.0088      0.5460
        H     -0.4576     -0.8996      1.1492
        H     -0.3766      0.8614      1.2066
        C     -1.0882      0.0718     -0.6703
        H     -0.8749      0.9665     -1.2703
        H     -0.9568     -0.7985     -1.3268
        H     -2.1474      0.1118     -0.3862
        """)
