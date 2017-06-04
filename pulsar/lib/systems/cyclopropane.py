import pulsar as psr
def load_ref_system():
    """ Returns cyclopropane as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C     -0.8357     -0.2326      0.1146
        C      0.2153      0.8471     -0.0528
        C      0.6207     -0.6142     -0.0617
        H      0.2520      1.4207     -0.9760
        H      0.4803      1.4731      0.7962
        H      0.9427     -1.0765     -0.9917
        H      1.1713     -1.0242      0.7809
        H     -1.3112     -0.3724      1.0824
        H     -1.5399     -0.4247     -0.6920
        """)
