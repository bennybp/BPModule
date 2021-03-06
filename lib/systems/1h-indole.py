import pulsar as psr
def load_ref_system():
    """ Returns 1h-indole as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      2.2544     -0.0924      0.0221
        C      1.7667     -1.4146      0.0776
        C      0.4146     -1.6846      0.0789
        C     -0.4799     -0.6069      0.0234
        C      1.4019      0.9893     -0.0332
        C      0.0242      0.7214     -0.0322
        C     -1.9138     -0.5276      0.0084
        C     -2.2626      0.8055     -0.0542
        H      3.3373      0.0703      0.0235
        H      2.4863     -2.2385      0.1202
        H      0.0351     -2.7098      0.1219
        H      1.7792      2.0153     -0.0763
        H     -2.5898     -1.3778      0.0412
        H     -3.2578      1.2492     -0.0821
        N     -1.0872      1.5825     -0.0797
        H     -1.0472      2.5668     -0.1237
        """)
