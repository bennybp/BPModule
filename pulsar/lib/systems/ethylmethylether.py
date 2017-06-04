import pulsar as psr
def load_ref_system():
    """ Returns ethylmethylether as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      2.3968      0.1143      1.3097
        C      1.8230      0.0672      0.3757
        H      2.0992      0.9413     -0.2285
        H      2.1534     -0.8247     -0.1727
        C      0.3365      0.0308      0.6731
        H      0.0707     -0.8514      1.2879
        H      0.0160      0.9303      1.2344
        O     -0.3430     -0.0275     -0.5759
        C     -1.7393     -0.0675     -0.4548
        H     -2.0816     -0.9567      0.0885
        H     -2.1374      0.8293      0.0355
        H     -2.0803     -0.1090     -1.4920
        """)
