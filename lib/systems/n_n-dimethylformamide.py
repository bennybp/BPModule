import pulsar as psr
def load_ref_system():
    """ Returns n_n-dimethylformamide as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C     -0.0117      1.0046      0.3304
        O     -1.1681      1.3725      0.3691
        N      0.3014     -0.3755      0.1297
        C      1.6473     -0.6630     -0.4111
        C     -0.7489     -1.2964     -0.3518
        H      0.8385      1.6834      0.5135
        H      2.3903      0.0188      0.0236
        H      1.6903     -0.5712     -1.5065
        H      1.9297     -1.6899     -0.1430
        H     -0.4255     -2.3311     -0.1779
        H     -0.9577     -1.1726     -1.4243
        H     -1.6797     -1.1280      0.2052
        """)
