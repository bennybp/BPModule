import pulsar as psr
def load_ref_system():
    """ Returns d-lactic_acid as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      2.4019     -0.4053      0.1274
        C      1.6220     -1.0718     -0.2619
        H      2.0483     -1.6336     -1.1034
        H      1.3790     -1.7961      0.5269
        C      0.4000     -0.2748     -0.7030
        H      0.6828      0.4386     -1.5171
        C     -0.1724      0.5594      0.4510
        O      0.2782      0.7719      1.5592
        O     -1.3456      1.1773      0.1697
        H     -1.6484      1.6656      0.9291
        O     -0.5478     -1.2044     -1.1799
        H     -1.2656     -0.7115     -1.5548
        """)
