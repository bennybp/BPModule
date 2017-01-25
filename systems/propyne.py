import pulsar as psr
def load_ref_system():
    """ Returns propyne as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C     -0.1488     -0.0004     -0.0352
        C     -1.3084     -0.0034     -0.3090
        C      1.2454      0.0033      0.2941
        H     -2.3439     -0.0062     -0.5534
        H      1.3942      0.2722      1.3481
        H      1.7883      0.7290     -0.3256
        H      1.6857     -0.9885      0.1272
        """)
