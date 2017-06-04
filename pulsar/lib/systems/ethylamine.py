import pulsar as psr
def load_ref_system():
    """ Returns ethylamine as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      1.2277      0.4098     -0.0270
        C      0.0679     -0.5616      0.0680
        N     -1.2273      0.1369     -0.0900
        H      1.2237      0.9656     -0.9741
        H      1.2144      1.1497      0.7853
        H      0.1336     -1.1313      1.0206
        H      0.1307     -1.3142     -0.7430
        H     -1.2998      0.8711      0.5835
        H     -1.9719     -0.5117      0.0544
        H      2.1852     -0.1236      0.0348
        """)
