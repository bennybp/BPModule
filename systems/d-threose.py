import pulsar as psr
def load_ref_system():
    """ Returns d-threose as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      0.3911     -0.4260     -0.0122
        C     -0.4512      0.8541      0.2335
        O     -0.2541     -1.1666     -1.0292
        C      0.5491     -1.2281      1.3018
        O     -1.7693      0.5943      0.6421
        C     -0.4295      1.7451     -1.0158
        O      1.3951     -2.3338      1.1452
        O      0.5399      2.3994     -1.3173
        H      1.3966     -0.1203     -0.3959
        H     -0.0423      1.4070      1.1115
        H      0.0799     -2.0532     -0.9829
        H     -0.4047     -1.6835      1.6239
        H      0.9038     -0.5647      2.1138
        H     -2.1697      0.0275     -0.0064
        H     -1.3428      1.7729     -1.6324
        H      2.2737     -2.0049      0.9992
        """)
