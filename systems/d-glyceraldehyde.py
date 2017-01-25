import pulsar as psr
def load_ref_system():
    """ Returns d-glyceraldehyde as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C     -0.5458      1.3514      0.3891
        C      0.3554      0.1256      0.2630
        C     -0.5349     -1.1410      0.2914
        O      1.2026      0.1342     -0.8570
        O      0.1813     -2.2999      0.6057
        O     -0.7959      2.0852     -0.5371
        H     -0.9793      1.5336      1.3852
        H      1.0877      0.1189      1.1042
        H     -1.0733     -1.2658     -0.6691
        H     -1.2846     -1.0805      1.0994
        H      0.6786      0.3524     -1.6190
        H      0.8825     -2.3833     -0.0294
        """)
