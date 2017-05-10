import pulsar as psr
def load_ref_system():
    """ Returns uracil as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      1.6320      0.2951     -0.0637
        C      1.4462     -1.0375      0.0414
        N      0.1579     -1.5881      0.1158
        C     -1.0000     -0.7548      0.0823
        N     -0.8139      0.6462     -0.0280
        C      0.4966      1.2211     -0.1047
        O     -2.1159     -1.2556      0.1484
        O      0.5471      2.4364     -0.1977
        H      2.2820     -1.7492      0.0738
        H      0.0433     -2.5733      0.1927
        H     -1.6241      1.2299     -0.0514
        H      2.6363      0.7286     -0.1219
        """)