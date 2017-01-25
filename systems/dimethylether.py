import pulsar as psr
def load_ref_system():
    """ Returns dimethylether as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      1.5296      0.1437      1.3352
        C      1.1625      0.0164      0.3143
        H      1.5371      0.8255     -0.3240
        H      1.4986     -0.9463     -0.0891
        O     -0.2346      0.0661      0.4606
        C     -0.9435     -0.0781     -0.7444
        H     -0.7125      0.7246     -1.4549
        H     -0.7510     -1.0473     -1.2200
        H     -1.9887     -0.0143     -0.4335
        """)
