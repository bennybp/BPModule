import pulsar as psr
def load_ref_system():
    """ Returns l-serine as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      0.1700     -1.0189      1.2696
        C      0.5031     -0.2242      0.5537
        N      1.9822     -0.2976      0.4909
        H      2.2604     -1.2011      0.1738
        H      2.3386      0.4002     -0.1279
        C      0.0918      1.1599      1.1000
        H      0.6849      1.4048      2.0036
        H      0.2754      1.9567      0.3514
        O     -1.2768      1.0939      1.4269
        H     -1.5210      1.9555      1.7357
        C     -0.1403     -0.4556     -0.8110
        O      0.1059      0.1023     -1.8665
        O     -1.0793     -1.4231     -0.8544
        H     -1.4527     -1.4808     -1.7286
        """)
