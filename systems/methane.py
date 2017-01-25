import pulsar as psr
def load_ref_system():
    """ Returns methane as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      0.5288      0.1610      0.9359
        C      0.0000      0.0000      0.0000
        H      0.2051      0.8240     -0.6786
        H      0.3345     -0.9314     -0.4496
        H     -1.0685     -0.0537      0.1921
        """)
