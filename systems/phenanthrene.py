import pulsar as psr
def load_ref_system():
    """ Returns phenanthrene as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
      C         -0.00000        0.85983        1.40677
      C         -0.00000       -0.38534        0.71577
      C          0.00000       -0.38534       -0.71577
      C          0.00000        0.85983       -1.40677
      C         -0.00000        0.88997        2.81073
      C         -0.00000       -0.29579        3.54109
      C         -0.00000       -1.52195        2.88208
      C         -0.00000       -1.57625        1.48602
      C         -0.00000       -1.57625       -1.48602
      C          0.00000       -1.52195       -2.88208
      C          0.00000       -0.29579       -3.54109
      C          0.00000        0.88997       -2.81073
      H         -0.00000        1.83522        3.34096
      H         -0.00000       -0.26405        4.62305
      H         -0.00000       -2.43967        3.45635
      H         -0.00000       -2.55408        1.03701
      H         -0.00000       -2.55408       -1.03701
      H          0.00000       -2.43967       -3.45635
      H          0.00000       -0.26405       -4.62305
      H          0.00000        1.83522       -3.34096
      C          0.00000        2.06397       -0.69467
      C          0.00000        2.06397        0.69467
      H          0.00000        3.01209       -1.21991
      H          0.00000        3.01209        1.21991
        """)
