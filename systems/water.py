import pulsar as psr
def load_ref_system():
    """ Returns water as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      0.5080     -0.0515      0.7596
        O      0.0248      0.0050     -0.0574
        H     -0.9021     -0.0284      0.1521
        """)
