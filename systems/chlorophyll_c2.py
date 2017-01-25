import pulsar as psr
def load_ref_system():
    """ Returns chlorophyll_c2 as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
      C         -2.51105        2.48309       -0.00367
      C         -1.01315        4.06218        0.09798
      C          0.16582        4.71018        0.03445
      C          1.49771        4.10555       -0.16166
      C          3.11156        2.64885       -0.24011
      C          3.76129        1.46796       -0.14139
      C          3.09159        0.16424       -0.04831
      C          1.53984       -1.29933       -0.10261
      C         -0.91181       -1.59853       -0.35248
      C         -2.45607       -0.05240       -0.35298
      C         -3.10620        1.27291       -0.16460
      N          1.75017        2.78429       -0.23961
      N          1.76493        0.00621       -0.16383
      N         -1.13225       -0.25365       -0.35470
      C         -3.20202        3.66639        0.28881
      C          2.68521        4.82968       -0.22081
      C          3.68747       -1.09005        0.15938
      C         -2.24542        4.67945        0.36245
      C         -2.41991        6.13752        0.71407
      C         -3.56473        6.73948        1.06129
      C          3.72098        3.91621       -0.29248
      N         -1.17823        2.72953       -0.07212
      Mg         0.26793        1.27490       -0.35377
      C          0.34760       -2.11926       -0.19964
      C          2.64511       -1.99016        0.09433
      C          2.31747       -3.38932        0.17127
      O          3.13845       -4.27117        0.35919
      C          0.80633       -3.58955       -0.01558
      O         -0.47000       -3.54277        1.99390
      C          0.19951       -4.22772        1.23456
      O          0.33390       -5.55184        1.51538
      C         -2.14828       -2.27838       -0.46597
      C         -2.34627       -3.76961       -0.59495
      C         -3.52338       -4.42976       -0.61516
      C         -3.58867       -5.90449       -0.76226
      O         -2.56916       -6.57072       -0.87406
      O         -4.79233       -6.51526       -0.77059
      C         -3.12715       -1.27243       -0.46107
      C          0.83624       -6.50254        0.65250
      C          5.12089       -1.42417        0.42504
      C         -4.62172       -1.44902       -0.55794
      C         -4.68427        3.81267        0.53984
      C          2.86086        6.32939       -0.22433
      C          6.20198        3.61878       -0.78465
      C          5.15458        4.35926       -0.39637
      H          0.16297        5.77206        0.17065
      H          4.82596        1.44735       -0.07176
      H         -4.17158        1.24062       -0.09244
      H         -1.55035        6.77752        0.72933
      H         -4.51438        6.24702        1.11665
      H         -3.54680        7.79385        1.31901
      H          0.60711       -4.17663       -0.93384
      H         -1.47776       -4.37959       -0.70310
      H         -4.46318       -3.92775       -0.52160
      H         -4.86588       -7.47619       -0.86197
      H          0.78374       -7.49457        1.14518
      H          1.89335       -6.30524        0.40486
      H          0.22776       -6.53794       -0.27557
      H          5.75783       -0.51879        0.44869
      H          5.50014       -2.12026       -0.34842
      H          5.18953       -1.91331        1.42119
      H         -5.18859       -0.50460       -0.59248
      H         -4.99451       -2.00986        0.32226
      H         -4.86783       -1.97442       -1.50531
      H         -5.27115        2.88458        0.45319
      H         -5.12053        4.53158       -0.18161
      H         -4.83661        4.15056        1.58664
      H          1.91301        6.89359       -0.19061
      H          3.38213        6.63177       -1.15809
      H          3.47321        6.63697        0.64765
      H          7.18160        4.08465       -0.84048
      H          6.13795        2.59183       -1.09149
      H          5.36680        5.40361       -0.19182
        """)
