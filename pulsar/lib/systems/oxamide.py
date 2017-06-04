import pulsar as psr
def load_ref_system():
    """ Returns oxamide as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      0.7262      0.0488      0.2268
        C     -0.7262     -0.0486     -0.2267
        N      1.2793     -1.0351      0.8826
        N     -1.2794      1.0349     -0.8829
        O      1.4338      1.0371      0.0420
        O     -1.4337     -1.0370     -0.0418
        H      0.7481     -1.8526      1.0540
        H      2.2201     -1.0088      1.1924
        H     -2.2204      1.0084     -1.1924
        H     -0.7484      1.8525     -1.0543
        """)
