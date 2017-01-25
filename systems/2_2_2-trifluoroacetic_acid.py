import pulsar as psr
def load_ref_system():
    """ Returns 2_2_2-trifluoroacetic_acid as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      0.5119     -0.1804      0.1034
        C     -0.8986      0.4734     -0.1579
        O     -1.1639      1.6412     -0.0517
        O     -1.9253     -0.3042     -0.5347
        F      1.4648      0.6901      0.4768
        F      0.9963     -0.8142     -0.9854
        F      0.4730     -1.1227      1.0689
        H     -1.6614     -1.2137     -0.6023
        """)
