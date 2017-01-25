import pulsar as psr
def load_ref_system():
    """ Returns isobutyric_acid as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      2.6683      0.0292      1.0036
        C      1.9584     -0.0413      0.1691
        H      2.1131      0.8362     -0.4732
        H      2.2217     -0.9295     -0.4192
        C      0.5313     -0.1179      0.6812
        H      0.4365     -1.0309      1.3210
        C      0.1906      1.1090      1.5126
        H      0.8720      1.2020      2.3685
        H      0.2759      2.0326      0.9245
        H     -0.8322      1.0612      1.9093
        C     -0.4647     -0.2431     -0.4653
        O     -0.3069      0.0059     -1.6407
        O     -1.7272     -0.6845     -0.2445
        H     -1.8681     -0.8515      0.6791
        """)
