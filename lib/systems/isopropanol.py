import pulsar as psr
def load_ref_system():
    """ Returns isopropanol as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      2.0186     -0.2175      0.7985
        C      1.4201     -0.2502     -0.1210
        H      1.6783      0.6389     -0.7114
        H      1.7345     -1.1325     -0.6927
        C     -0.0726     -0.3145      0.1833
        H     -0.2926     -1.2317      0.7838
        O     -0.3758      0.8195      0.9774
        H     -1.3159      0.8236      1.0972
        C     -0.8901     -0.3435     -1.1071
        H     -0.7278      0.5578     -1.7131
        H     -0.6126     -1.2088     -1.7220
        H     -1.9673     -0.4150     -0.9062
        """)
