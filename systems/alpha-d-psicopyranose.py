import pulsar as psr
def load_ref_system():
    """ Returns alpha-d-psicopyranose as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        O     -0.8523      2.0191      0.6536
        C     -0.6803      0.9081     -0.2279
        O      0.4856      1.2959     -0.9631
        C      1.3351      0.2704     -1.5196
        C      1.6070     -0.9064     -0.5712
        O      2.4885     -0.5317      0.4760
        C      0.2766     -1.4581      0.0015
        O      0.5344     -2.5036      0.9184
        C     -0.5184     -0.3287      0.6918
        O      0.2579      0.0386      1.8423
        C     -1.8746      0.8337     -1.2049
        O     -2.9669      0.1442     -0.6022
        H      0.8797     -0.0602     -2.4691
        H      2.2621      0.8478     -1.7198
        H     -1.5147     -0.6922      1.0428
        H     -0.3440     -1.9497     -0.7850
        H      2.1772     -1.7145     -1.0918
        H     -1.6258      0.2177     -2.0903
        H     -2.1802      1.8431     -1.5330
        H     -0.2049      2.7370      0.4201
        H     -0.1042      0.8825      2.2275
        H      1.1229     -2.1676      1.6503
        H      2.0361      0.1134      1.0977
        H     -3.4053      0.7244      0.0631
        """)
