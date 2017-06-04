import pulsar as psr
def load_ref_system():
    """ Returns l-malic_acid as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        O      0.8770     -0.3940      2.5072
        C      1.1528      0.3172      1.5589
        O      2.4009      0.8395      1.5431
        C      0.2810      0.7244      0.3974
        C     -0.2678     -0.5055     -0.3483
        C     -1.3701     -0.0328     -1.3037
        O      0.7254     -1.2169     -1.0331
        H      0.8326      1.3899     -0.2994
        H     -0.5598      1.3259      0.7973
        H     -0.6577     -1.2711      0.3646
        H      1.0774     -0.6587     -1.7175
        H      2.8923      0.5394      2.3015
        O     -2.6231     -0.0957     -0.8077
        O     -1.2480      0.3930     -2.4374
        H     -3.2528      0.2020     -1.4573
        """)
