import pulsar as psr
def load_ref_system():
    """ Returns l-aspartic_acid as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        N     -0.5386     -1.3898      0.9506
        C      0.1294     -0.5973     -0.1082
        C      1.6120     -0.9474     -0.2410
        O      1.8554     -2.1093     -0.8893
        O      2.5913     -0.3490      0.1622
        C     -0.0316      0.9010      0.1754
        C     -1.4539      1.3518     -0.0491
        O     -2.1684      1.1761     -1.0199
        O     -1.9885      2.0787      0.9585
        H     -0.0808     -1.2591      1.8278
        H     -0.5412     -2.3564      0.7070
        H     -0.3817     -0.8459     -1.0750
        H      2.7930     -2.2689     -0.9403
        H      0.6201      1.4867     -0.5050
        H      0.3085      1.1503      1.2029
        H     -2.8830      2.3257      0.7442
        """)
