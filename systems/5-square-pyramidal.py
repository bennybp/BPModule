import pulsar as psr
def load_ref_system():
    """ Returns 5-square-pyramidal as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        S      0.0445      0.1425     -0.0192
        F     -0.9388     -0.4312     -1.2960
        F      0.6222     -0.5695      1.4248
        F      0.5559      1.7611     -0.2287
        F     -1.4362      0.0019      0.8255
        F      1.1217     -1.0027     -0.6932
        """)
