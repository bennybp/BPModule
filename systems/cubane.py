import pulsar as psr
def load_ref_system():
    """ Returns cubane as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      1.2455      0.5367     -0.0729
        C      0.9239     -0.9952      0.0237
        C     -0.1226     -0.7041      1.1548
        C      0.1989      0.8277      1.0582
        C      0.1226      0.7042     -1.1548
        C     -0.9239      0.9952     -0.0237
        C     -1.2454     -0.5367      0.0729
        C     -0.1989     -0.8277     -1.0582
        H      2.2431      0.9666     -0.1313
        H      1.6638     -1.7924      0.0426
        H     -0.2209     -1.2683      2.0797
        H      0.3583      1.4907      1.9059
        H      0.2208      1.2681     -2.0799
        H     -1.6640      1.7922     -0.0427
        H     -2.2430     -0.9665      0.1313
        H     -0.3583     -1.4906     -1.9058
        """)
