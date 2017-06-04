import pulsar as psr
def load_ref_system():
    """ Returns ethanethiol as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      1.7541      0.0174      0.1849
        C      0.4078      0.6686      0.3759
        S     -1.0026     -0.2781     -0.2746
        H      1.8249     -0.9392      0.7204
        H      2.5581      0.6628      0.5639
        H      1.9675     -0.1823     -0.8739
        H      0.3660      1.6368     -0.1597
        H      0.2493      0.9036      1.4442
        H     -0.8337     -1.4094      0.3591
        """)
