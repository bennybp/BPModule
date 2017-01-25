import pulsar as psr
def load_ref_system():
    """ Returns acetaldehyde as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      1.8870     -0.2640      0.6645
        C      1.2203     -0.2396     -0.2074
        H      1.4534      0.6668     -0.7811
        H      1.4662     -1.1053     -0.8363
        C     -0.2092     -0.3158      0.2358
        H     -0.5002     -1.2065      0.8165
        O     -1.0303      0.5372     -0.0127
        """)
