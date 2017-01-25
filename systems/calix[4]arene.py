import pulsar as psr
def load_ref_system():
    """ Returns calix[4]arene as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
      H          4.41501       -1.39863        1.53367
      C          3.54176       -1.84348        1.06151
      C          2.97735       -2.98350        1.63236
      H          3.41234       -3.41234        2.53143
      C          1.84515       -3.56215        1.06282
      H          1.40645       -4.43820        1.53465
      C          1.26706       -3.01550       -0.09239
      C         -0.00399       -3.62570       -0.64888
      C          1.85595       -1.87273       -0.66097
      C          2.99247       -1.26699       -0.09588
      C         -2.99247        1.26699       -0.09588
      C         -3.54176        1.84348        1.06151
      H         -4.41501        1.39863        1.53367
      C         -2.97735        2.98350        1.63236
      H         -3.41234        3.41234        2.53143
      C         -1.84515        3.56215        1.06282
      H         -1.40645        4.43820        1.53465
      C         -1.26706        3.01550       -0.09239
      C          0.00399        3.62570       -0.64888
      C         -1.85595        1.87273       -0.66097
      C          3.62570       -0.00399       -0.64888
      C          3.01550        1.26706       -0.09239
      C          1.87273        1.85595       -0.66097
      C          1.26699        2.99247       -0.09588
      C          1.84348        3.54176        1.06151
      H          1.39863        4.41501        1.53367
      C          2.98350        2.97735        1.63236
      H          3.41234        3.41234        2.53143
      C          3.56215        1.84515        1.06282
      H          4.43820        1.40645        1.53465
      H         -3.41234       -3.41234        2.53143
      C         -2.98350       -2.97735        1.63236
      C         -3.56215       -1.84515        1.06282
      H         -4.43820       -1.40645        1.53465
      C         -3.01550       -1.26706       -0.09239
      C         -3.62570        0.00399       -0.64888
      C         -1.87273       -1.85595       -0.66097
      C         -1.26699       -2.99247       -0.09588
      C         -1.84348       -3.54176        1.06151
      H         -1.39863       -4.41501        1.53367
      O         -1.26708        1.36876       -1.79270
      H         -1.55107        0.43221       -1.88490
      O          1.36876        1.26708       -1.79270
      H          0.43221        1.55107       -1.88490
      O          1.26708       -1.36876       -1.79270
      H          1.55107       -0.43221       -1.88490
      O         -1.36876       -1.26708       -1.79270
      H         -0.43221       -1.55107       -1.88490
      H         -0.01316       -4.70218       -0.42701
      H         -0.00318       -3.59228       -1.74550
      H          0.00318        3.59228       -1.74550
      H          0.01316        4.70218       -0.42701
      H          4.70218       -0.01316       -0.42701
      H          3.59228       -0.00318       -1.74550
      H         -3.59228        0.00318       -1.74550
      H         -4.70218        0.01316       -0.42701
        """)
