import pulsar as psr
def load_ref_system():
    """ Returns methanethiol as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      0.9156      0.6663      0.4484
        S     -0.4465     -0.2909     -0.2378
        H      0.9361      1.6286     -0.0785
        H      0.8018      0.8767      1.5172
        H     -0.3370     -1.3813      0.4734
        H      1.8925      0.1914      0.3082
        """)
