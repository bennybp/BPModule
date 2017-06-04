import pulsar as psr
def load_ref_system():
    """ Returns 2_2_2-trichloroacetic_acid as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      0.1451     -0.0598      0.0004
        C     -1.0655      0.8302      0.1611
        O     -1.1170      1.9463      0.6341
       Cl      1.5973      0.7262      0.5897
       Cl     -0.0807     -1.5539      0.9000
        O     -2.2119      0.2759     -0.2895
       Cl      0.3812     -0.4610     -1.6953
        H     -2.9414      0.8748     -0.1647
        """)
