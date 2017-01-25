import pulsar as psr
def load_ref_system():
    """ Returns ethyl_carbamate as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        N      0.7546      1.9536      0.5417
        C     -0.1582      0.9277      0.3228
        O      0.4747     -0.1973     -0.1242
        C     -0.3450     -1.3380     -0.3928
        C      0.5886     -2.4338     -0.8689
        O     -1.3665      1.0149      0.5042
        H      1.7235      1.8327      0.3796
        H      0.4288      2.8290      0.8687
        H     -1.0987     -1.0799     -1.1602
        H     -0.8849     -1.6309      0.5272
        H      1.3411     -2.6942     -0.1129
        H      1.1281     -2.1501     -1.7821
        H      0.0149     -3.3423     -1.0919
        """)
