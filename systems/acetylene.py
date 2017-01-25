import pulsar as psr
def load_ref_system():
    """ Returns acetylene as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      0.5788      0.0124      0.1371
        C     -0.5788     -0.0124     -0.1371
        H      1.6143      0.0346      0.3826
        H     -1.6143     -0.0345     -0.3823
        """)
