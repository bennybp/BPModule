import pulsar as psr
def load_ref_system():
    """ Returns oxalic_acid as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        O      1.6000      0.7900      0.1124
        C      0.7377     -0.0100     -0.1829
        O      1.1026     -1.0691     -0.9344
        H      2.0335     -1.0284     -1.1325
        C     -0.7376      0.0101      0.1829
        O     -1.5999     -0.7898     -0.1129
        O     -1.1027      1.0688      0.9349
        H     -2.0339      1.0283      1.1320
        """)
