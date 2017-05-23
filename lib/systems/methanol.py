import pulsar as psr
def load_ref_system():
    """ Returns methanol as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      1.2001      0.0363      0.8431
        C      0.7031      0.0083     -0.1305
        H      0.9877      0.8943     -0.7114
        H      1.0155     -0.8918     -0.6742
        O     -0.6582     -0.0067      0.1730
        H     -1.1326     -0.0311     -0.6482
        """)
