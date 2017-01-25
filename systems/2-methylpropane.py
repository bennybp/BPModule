import pulsar as psr
def load_ref_system():
    """ Returns 2-methylpropane as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      2.0300     -0.3293      0.7396
        C      1.4193     -0.2197     -0.1658
        H      1.6626      0.7549     -0.6103
        H      1.7384     -0.9933     -0.8762
        C     -0.0627     -0.3358      0.1493
        H     -0.2505     -1.3424      0.5964
        C     -0.4771      0.7225      1.1578
        H      0.0950      0.6316      2.0902
        H     -0.3132      1.7367      0.7692
        H     -1.5414      0.6366      1.4126
        C     -0.8881     -0.2133     -1.1206
        H     -0.7415      0.7614     -1.6053
        H     -0.6160     -0.9869     -1.8502
        H     -1.9607     -0.3179     -0.9117
        """)
