import pulsar as psr
def load_ref_system():
    """ Returns 3-trigonal-planar as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        B      0.0000      0.0000      0.0000
        H      1.1490      0.0000      0.0000
        H     -0.5745     -0.9951      0.0000
        H     -0.5745      0.9951      0.0000
        """)
