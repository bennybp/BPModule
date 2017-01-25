import pulsar as psr
def load_ref_system():
    """ Returns formic_acid as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      0.7910     -0.0921      1.3425
        C      0.3208     -0.0411      0.3549
        O      0.8042     -0.1358     -0.7518
        O     -1.0065      0.1597      0.4283
        H     -1.4043      0.2023     -0.4370
        """)
