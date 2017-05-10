import pulsar as psr
def load_ref_system():
    """ Returns 1h-pyrazole as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      1.0934      0.4724     -0.0376
        C      0.8289     -0.8959      0.0055
        C     -0.5823     -1.0383      0.0414
        H      2.0511      0.9905     -0.0730
        H      1.5593     -1.6999      0.0104
        H     -1.1801     -1.9470      0.0799
        N     -0.1470      1.1002     -0.0266
        N     -1.1526      0.1957      0.0208
        H     -0.3393      2.0670     -0.0484
        """)