import pulsar as psr
def load_ref_system():
    """ Returns cytosine as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C     -1.0686      1.2235      0.0001
        C      0.1809      1.7655      0.0229
        N      1.3002      0.9431      0.0346
        C      1.1549     -0.4959      0.0315
        N     -0.1540     -1.0401      0.0156
        C     -1.2006     -0.2143     -0.0173
        O      2.1744     -1.1638      0.0487
        N     -2.4618     -0.8153     -0.1637
        H     -2.5061     -1.7841      0.0579
        H     -3.2005     -0.2849      0.2368
        H      0.3473      2.8507      0.0324
        H     -1.9578      1.8616     -0.0159
        H      2.2062      1.3540      0.0507
        """)
