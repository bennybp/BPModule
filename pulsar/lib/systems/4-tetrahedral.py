import pulsar as psr
def load_ref_system():
    """ Returns 4-tetrahedral as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      0.0000     -0.0000      0.0000
        H      1.1400     -0.0000      0.0000
        H     -0.3805     -1.0746      0.0000
        H     -0.3799      0.5375      0.9308
        H     -0.3797      0.5372     -0.9310
        """)
