import pulsar as psr
def load_ref_system():
    """ Returns 4-planar as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
       Pt     -0.0000     -0.0000     -0.0000
       Cl     -2.1782      0.7530     -0.4981
       Cl      2.1782     -0.7530      0.4981
       Cl      0.8994      1.6933     -1.3729
       Cl     -0.8994     -1.6933      1.3729
        """)
