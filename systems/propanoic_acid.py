import pulsar as psr
def load_ref_system():
    """ Returns propanoic_acid as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      2.0037     -1.1917      0.1558
        C      1.4607     -1.1002     -0.7940
        H      1.8667     -0.2289     -1.3241
        H      1.6970     -1.9882     -1.3941
        C     -0.0328     -0.9826     -0.5799
        H     -0.5592     -0.9169     -1.5529
        H     -0.4245     -1.8900     -0.0796
        C     -0.3558      0.2190      0.2726
        O     -0.3823      0.3115      1.4865
        O     -0.6582      1.3495     -0.4082
        H     -0.8403      2.0580      0.2015
        """)
