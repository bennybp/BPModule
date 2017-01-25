import pulsar as psr
def load_ref_system():
    """ Returns thymine as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      1.2968      0.1931     -0.0745
        C      1.0973     -1.1460     -0.0338
        N     -0.1920     -1.6945      0.0478
        C     -1.3449     -0.8600      0.0919
        N     -1.1526      0.5437      0.0518
        C      0.1547      1.1225     -0.0321
        O     -2.4624     -1.3576      0.1626
        C      2.6589      0.7724     -0.1648
        O      0.1900      2.3418     -0.0598
        H      1.9295     -1.8636     -0.0624
        H     -0.3064     -2.6823      0.0746
        H     -1.9604      1.1307      0.0834
        H      3.4393      0.0002     -0.1449
        H      2.8463      1.4610      0.6704
        H      2.7763      1.3444     -1.0955
        """)
