import pulsar as psr
def load_ref_system():
    """ Returns acetic_acid as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      1.9933     -0.0696      0.7114
        C      1.4621     -0.1360     -0.2471
        H      1.7972      0.7009     -0.8739
        H      1.7611     -1.0706     -0.7384
        C     -0.0193     -0.0845     -0.0386
        O     -0.8655     -0.9092     -0.3334
        O     -0.4776      1.0398      0.5632
        H     -1.4239      0.9943      0.6582
        """)
