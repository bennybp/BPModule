import pulsar as psr
def load_ref_system():
    """ Returns d-malic_acid as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        O      1.3069     -0.1610      2.2485
        C      0.6981      0.7289      1.6819
        O      0.4491      1.8376      2.4171
        H      0.7906      1.7274      3.2992
        C      0.2093      0.7959      0.2569
        H     -0.6099      1.5393      0.1616
        H      1.0464      1.1780     -0.3622
        C     -0.2523     -0.5787     -0.2519
        H      0.4710     -1.3763      0.0568
        O     -1.5082     -0.8207      0.3421
        H     -1.8666     -1.6089     -0.0451
        C     -0.3426     -0.6060     -1.7815
        O     -1.2648     -1.0019     -2.4702
        O      0.7546     -0.1851     -2.4491
        H      0.6113     -0.2569     -3.3881
        """)
