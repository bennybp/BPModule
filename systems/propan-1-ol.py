import pulsar as psr
def load_ref_system():
    """ Returns propan-1-ol as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      2.2987      0.0288      1.6134
        C      1.8384     -0.0177      0.6184
        H      2.2379      0.8212      0.0329
        H      2.1852     -0.9435      0.1400
        C      0.3306      0.0328      0.7113
        H     -0.0417     -0.8023      1.3373
        H      0.0106      0.9572      1.2317
        C     -0.2986     -0.0320     -0.6787
        H      0.0473      0.8134     -1.3064
        H     -0.0064     -0.9661     -1.1988
        O     -1.6978      0.0198     -0.5178
        H     -2.0701     -0.0214     -1.3878
        """)
