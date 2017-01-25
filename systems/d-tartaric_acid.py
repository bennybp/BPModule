import pulsar as psr
def load_ref_system():
    """ Returns d-tartaric_acid as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H     -1.5157     -0.6176      0.3606
        C     -0.4649     -0.5240      0.7353
        O     -0.4105     -0.7172      2.1337
        H     -0.9548     -1.4685      2.3276
        C      0.0621      0.9196      0.4718
        H      1.1390      0.9923      0.7676
        O     -0.7073      1.8739      1.1705
        H     -0.5294      1.7480      2.0934
        C     -0.0999      1.2950     -1.0075
        O     -0.9181      0.8895     -1.8111
        O      0.7952      2.2010     -1.4512
        H      0.6357      2.4005     -2.3686
        C      0.4040     -1.5841      0.0468
        O      1.4525     -1.4622     -0.5541
        O     -0.0911     -2.8396      0.1456
        H      0.4825     -3.4574     -0.2972
        """)
