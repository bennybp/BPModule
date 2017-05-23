import pulsar as psr
def load_ref_system():
    """ Returns formaldehyde as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      1.0686     -0.1411      1.0408
        C      0.5979      0.0151      0.0688
        H      1.2687      0.2002     -0.7717
        O     -0.5960     -0.0151     -0.0686
        """)
