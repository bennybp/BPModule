import pulsar as psr
def load_ref_system():
    """ Returns ammonia as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        N     -0.0353     -0.0440      0.0285
        H      0.2658      0.6496      0.6822
        H      0.7774     -0.4532     -0.3850
        H     -0.5522      0.4148     -0.6935
        """)
