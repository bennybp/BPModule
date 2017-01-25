import pulsar as psr
def load_ref_system():
    """ Returns 5-trigonal-bipyramidal as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        P      0.0000     -0.0011      0.0000
       Cl      0.0000      2.0091      0.0000
       Cl      1.7511     -0.9922      0.0000
       Cl     -1.7511     -0.9921      0.0000
       Cl      0.0000     -0.0120      2.1193
       Cl      0.0000     -0.0120     -2.1193
        """)
