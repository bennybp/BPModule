import pulsar as psr
def load_ref_system():
    """ Returns cyclobutane as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C     -0.8058     -0.7366      0.0748
        C     -0.7395      0.8061     -0.0273
        C      0.8059      0.7367     -0.0745
        H     -1.2036      1.2228     -0.9269
        H     -1.1440      1.3387      0.8393
        C      0.7394     -0.8063      0.0271
        H      1.2494      1.1143     -1.0010
        H      1.3093      1.2267      0.7649
        H      1.1448     -1.3373     -0.8404
        H      1.2032     -1.2252      0.9261
        H     -1.2501     -1.1129      1.0022
        H     -1.3087     -1.2276     -0.7648
        """)
