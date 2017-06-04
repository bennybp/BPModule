import pulsar as psr
def load_ref_system():
    """ Returns tetrahydrofuran as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      0.1567      1.3017     -0.0036
        C     -1.1348      0.4835      0.0550
        O     -0.8174     -0.8933     -0.1008
        C      0.5820     -1.0875      0.0551
        C      1.2827      0.2712     -0.0063
        H      0.2345      1.9940      0.8539
        H      0.1895      1.9405     -0.9047
        H     -1.8441      0.7135     -0.7563
        H     -1.6682      0.5968      1.0150
        H      0.8736     -1.7752     -0.7550
        H      0.7424     -1.6069      1.0159
        H      1.9118      0.3606     -0.9103
        H      1.9694      0.4097      0.8480
        """)
