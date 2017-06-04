import pulsar as psr
def load_ref_system():
    """ Returns butanone as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      1.8315     -0.5838     -0.0465
        C      0.5005      0.1194      0.0287
        C     -0.7017     -0.7997     -0.0095
        C     -2.0319     -0.0893     -0.0850
        O      0.4197      1.3291      0.1161
        H      1.9387     -1.0946     -1.0125
        H      1.9124     -1.3434      0.7418
        H      2.6813      0.1026      0.0625
        H     -0.6629     -1.4440      0.8918
        H     -0.5864     -1.4846     -0.8733
        H     -2.1884      0.5739      0.7758
        H     -2.8602     -0.8091     -0.1027
        H     -2.1111      0.5282     -0.9892
        """)
