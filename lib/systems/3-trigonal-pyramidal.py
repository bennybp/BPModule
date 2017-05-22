import pulsar as psr
def load_ref_system():
    """ Returns 3-trigonal-pyramidal as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        N     -0.0221      0.0312     -0.0541
        H      1.0979      0.0312     -0.0541
        H     -0.3959     -1.0245     -0.0541
        H     -0.3953      0.5593      0.8604
        """)
