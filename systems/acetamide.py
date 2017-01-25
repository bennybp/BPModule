import pulsar as psr
def load_ref_system():
    """ Returns acetamide as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      1.3538     -0.5732     -0.1812
        C      0.0125      0.0778      0.0216
        N     -1.1331     -0.6966     -0.1780
        O     -0.1619      1.2457      0.3516
        H      2.1721      0.1349      0.0048
        H      1.4658     -0.9442     -1.2088
        H      1.4919     -1.4234      0.5001
        H     -2.0381     -0.3116     -0.0561
        H     -1.0575     -1.6463     -0.4448
        """)
