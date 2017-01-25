import pulsar as psr
def load_ref_system():
    """ Returns phenol as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      1.7344      0.1731      2.1387
        C      1.1780      0.0989      1.1986
        C      1.8626      0.0162     -0.0086
        H      2.9570      0.0254     -0.0180
        C      1.1586     -0.0783     -1.2063
        H      1.7026     -0.1432     -2.1544
        C     -0.2296     -0.0912     -1.2096
        H     -0.7855     -0.1655     -2.1508
        C     -0.9117     -0.0073      0.0125
        C     -0.2116      0.0881      1.2224
        H     -0.7442      0.1538      2.1778
        O     -2.2783     -0.0249     -0.0584
        H     -2.6165      0.0372      0.8262
        """)
