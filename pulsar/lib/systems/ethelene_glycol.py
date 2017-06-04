import pulsar as psr
def load_ref_system():
    """ Returns ethelene_glycol as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
      C          0.00000       -0.49575       -0.57818
      C          0.00000        0.49575        0.57818
      O          0.00000       -0.20343        1.79079
      H          0.00000        0.48551        2.50516
      O          0.00000        0.20343       -1.79079
      H          0.00000       -0.48551       -2.50516
      H         -0.91185       -1.13063       -0.50888
      H          0.91185       -1.13063       -0.50888
      H         -0.91185        1.13063        0.50888
      H          0.91185        1.13063        0.50888
        """)
