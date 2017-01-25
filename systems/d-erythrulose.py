import pulsar as psr
def load_ref_system():
    """ Returns d-erythrulose as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C     -0.4968      0.5689     -0.1036
        C      0.5847     -0.4690      0.2373
        C     -0.0457      1.7239     -0.9899
        O     -1.6333      0.4827      0.3158
        C      0.0149     -1.7046      0.9742
        O      1.2110     -0.8125     -0.9827
        O     -0.8153      2.8847     -0.8698
        O      1.0351     -2.5599      1.4129
        H      1.3395      0.0458      0.8848
        H     -0.0153      1.3744     -2.0418
        H      0.9685      2.0642     -0.7153
        H     -0.5804     -2.3459      0.3001
        H     -0.6345     -1.3917      1.8134
        H      1.7050     -1.6070     -0.8254
        H      1.4786     -2.1314      2.1346
        H     -1.7270      2.6481     -0.9921
        """)
