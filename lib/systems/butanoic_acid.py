import pulsar as psr
def load_ref_system():
    """ Returns butanoic_acid as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      2.8976     -1.0100      1.9378
        C      2.4502     -1.0108      0.9357
        H      3.1450     -0.4874      0.2653
        H      2.3974     -2.0554      0.6008
        C      1.0860     -0.3585      0.9485
        H      0.4185     -0.8875      1.6573
        H      1.1660      0.6793      1.3276
        C      0.4710     -0.3563     -0.4439
        H      1.1318      0.1809     -1.1544
        H      0.3860     -1.3919     -0.8298
        C     -0.9035      0.2618     -0.4176
        O     -1.9382     -0.1856      0.0433
        O     -0.9995      1.4841     -0.9919
        H     -1.8950      1.8026     -0.9330
        """)
