import pulsar as psr
def load_ref_system():
    """ Returns 1_2-thiazole as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      1.1291      0.0795     -0.5259
        C      0.7115     -1.2207     -0.4748
        H      2.0789      0.4171     -0.9381
        H      1.2719     -2.0822     -0.8377
        S     -0.0500      1.1306      0.1514
        N     -1.1147     -0.1822      0.5074
        C     -0.5926     -1.3347      0.1299
        H     -1.1422     -2.2662      0.2851
        """)
