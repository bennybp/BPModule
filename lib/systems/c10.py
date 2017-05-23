import pulsar as psr
def load_ref_system():
    """ Returns c10 as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
      C          0.00000       -0.00000        1.67708
      C         -0.89581        0.89581        0.89581
      C         -0.00000        1.67708        0.00000
      C          0.89581        0.89581       -0.89581
      C          1.67708        0.00000       -0.00000
      C          0.00000       -1.67708       -0.00000
      C          0.89581       -0.89581        0.89581
      C         -1.67708       -0.00000        0.00000
      C         -0.89581       -0.89581       -0.89581
      C         -0.00000        0.00000       -1.67708
        """)
