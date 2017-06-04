import pulsar as psr
def load_ref_system():
    """ Returns benzoyl_chloride as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      1.3835      1.0239     -0.0000
        C      2.6581      0.4699     -0.0000
        C      2.8197     -0.9117     -0.0000
        C      1.7027     -1.7411     -0.0000
        C      0.4252     -1.1942     -0.0000
        C      0.2579      0.1929      0.0000
        H      1.2618      2.1144     -0.0000
        H      3.5362      1.1247     -0.0000
        H      3.8250     -1.3464     -0.0000
        H      1.8281     -2.8292     -0.0000
        H     -0.4505     -1.8565     -0.0000
        C     -1.0872      0.8033      0.0000
       Cl     -2.4420     -0.3546     -0.0000
        O     -1.3442      1.9805     -0.0000
        """)
