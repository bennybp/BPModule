import pulsar as psr
def load_ref_system():
    """ Returns pleaidene as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
      C         -0.00000       -1.27656        0.68709
      C         -0.00000       -2.44986        1.47816
      C         -0.00000       -2.40726        2.85991
      C         -0.00000       -1.19506        3.51182
      C         -0.00000        0.00000        2.78683
      C         -0.00000        0.00000        1.34398
      C          0.00000        1.27656        0.68709
      C          0.00000        2.44986        1.47816
      C         -0.00000        2.40726        2.85991
      C         -0.00000        1.19506        3.51182
      C         -0.00000       -1.56110       -0.79675
      C          0.00000        1.56110       -0.79675
      C          0.00000       -0.70194       -2.02078
      C          0.00000        0.70194       -2.02078
      C          0.00000       -1.38495       -3.25930
      C          0.00000       -0.69297       -4.46696
      C          0.00000        0.69297       -4.46696
      C          0.00000        1.38495       -3.25930
      H         -0.00000       -3.42863        1.01285
      H         -0.00000       -3.32771        3.43001
      H         -0.00000       -1.18531        4.59615
      H          0.00000        3.42863        1.01285
      H         -0.00000        3.32771        3.43001
      H         -0.00000        1.18531        4.59615
      H         -0.00000       -2.64677       -1.04288
      H          0.00000        2.64677       -1.04288
      H          0.00000       -2.46795       -3.29077
      H          0.00000       -1.23535       -5.40376
      H          0.00000        1.23535       -5.40376
      H          0.00000        2.46795       -3.29077
        """)
