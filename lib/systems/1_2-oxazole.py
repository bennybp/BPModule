import pulsar as psr
def load_ref_system():
    """ Returns 1_2-oxazole as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      1.0701      0.4341     -0.0336
        C      0.8115     -0.9049     -0.1725
        C     -0.6249     -1.0279     -0.0726
        H      1.9842      1.0231     -0.0364
        H      1.5156     -1.7176     -0.3255
        H     -1.2289     -1.9326     -0.1322
        O     -0.0873      1.1351      0.1422
        N     -1.1414      0.1776      0.1122
        """)
