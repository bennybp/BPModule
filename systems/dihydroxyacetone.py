import pulsar as psr
def load_ref_system():
    """ Returns dihydroxyacetone as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      1.2071     -0.8270     -0.0783
        C      0.0104      0.1089      0.0263
        C     -1.3376     -0.5418     -0.2542
        O      2.4209     -0.2007     -0.3711
        O      0.1224      1.2853      0.3118
        O     -2.4316      0.0877      0.3442
        H      1.0808     -1.5384     -0.9140
        H      1.2826     -1.4131      0.8595
        H     -1.3762     -1.5654      0.1597
        H     -1.4760     -0.6125     -1.3518
        H      2.5342      0.5214      0.2358
        H     -2.3882      1.0138      0.1371
        """)
