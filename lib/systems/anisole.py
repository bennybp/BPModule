import pulsar as psr
def load_ref_system():
    """ Returns anisole as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      0.6504      1.1978     -0.0297
        C      1.9454      0.7011     -0.0190
        C      2.1752     -0.6726      0.0122
        C      1.1032     -1.5553      0.0323
        C     -0.2051     -1.0813      0.0222
        C     -0.4284      0.2985     -0.0085
        H      0.4708      2.2786     -0.0544
        H      2.7923      1.3949     -0.0349
        H      3.2006     -1.0551      0.0208
        H      1.2845     -2.6349      0.0565
        H     -1.0364     -1.7948      0.0383
        O     -1.6672      0.9093     -0.0224
        C     -2.8002      0.0778      0.0128
        H     -2.8636     -0.5772     -0.8639
        H     -3.6200      0.8011     -0.0005
        H     -2.8459     -0.5255      0.9269
        """)
