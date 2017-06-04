import pulsar as psr
def load_ref_system():
    """ Returns d-erythrose as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      0.3861     -0.4742      0.1497
        C     -0.4866      0.8134      0.1116
        H      1.4067     -0.2490      0.5402
        O      0.6750     -0.9630     -1.1367
        C     -0.3051     -1.5537      1.0177
        O     -0.5739      1.4338      1.3689
        H     -1.5531      0.5590     -0.0928
        C      0.0635      1.7956     -0.9332
        O      0.5544     -2.6005      1.3664
        H     -1.2118     -1.9442      0.5132
        H      0.9005      2.4416     -0.6220
        O     -0.3979      1.8692     -2.0455
        H      0.8960     -2.9716      0.5616
        H     -0.6105     -1.1410      1.9951
        H      0.3100      1.6158      1.6660
        H     -0.1504     -1.1009     -1.5865
        """)
