import pulsar as psr
def load_ref_system():
    """ Returns 6-octahedral as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
      Fe      0.0000      0.0000      0.0000
      Cl     -1.4632     -0.3046     -1.7241
      Cl      1.4631      0.3045      1.7241
      Cl      0.6641      1.9824     -0.9138
      Cl     -0.6642     -1.9824      0.9138
      Cl     -1.6199      1.0878      1.1826
      Cl      1.6199     -1.0878     -1.1826
        """)
