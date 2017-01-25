import pulsar as psr
def load_ref_system():
    """ Returns 12-crown-4 as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
      C          1.62078       -1.47897        0.53812
      C          2.40215       -0.17144        0.64774
      O          1.50531        0.92352        0.69121
      C          1.47897        1.62078       -0.53812
      C          0.17144        2.40215       -0.64774
      O         -0.92352        1.50531       -0.69121
      C         -1.62078        1.47897        0.53812
      C         -2.40215        0.17144        0.64774
      O         -1.50531       -0.92352        0.69121
      C         -1.47897       -1.62078       -0.53812
      C         -0.17144       -2.40215       -0.64774
      O          0.92352       -1.50531       -0.69121
      H          2.33235       -2.33235        0.57773
      H          0.93605       -1.52139        1.41729
      H          2.97928       -0.17659        1.59658
      H          3.16121       -0.10299       -0.16818
      H          1.52139        0.93605       -1.41729
      H          2.33235        2.33235       -0.57773
      H          0.17659        2.97928       -1.59658
      H          0.10299        3.16121        0.16818
      H         -2.33235        2.33235        0.57773
      H         -0.93605        1.52139        1.41729
      H         -2.97928        0.17659        1.59658
      H         -3.16121        0.10299       -0.16818
      H         -1.52139       -0.93605       -1.41729
      H         -2.33235       -2.33235       -0.57773
      H         -0.10299       -3.16121        0.16818
      H         -0.17659       -2.97928       -1.59658
        """)
