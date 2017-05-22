import pulsar as psr
def load_ref_system():
    """ Returns ethene as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      1.1610      0.0661      1.0238
        C      0.6579     -0.0045      0.0639
        H      1.3352     -0.0830     -0.7815
        C     -0.6579      0.0045     -0.0639
        H     -1.3355      0.0830      0.7812
        H     -1.1608     -0.0661     -1.0239
        """)
