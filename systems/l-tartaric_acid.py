import pulsar as psr
def load_ref_system():
    """ Returns l-tartaric_acid as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      1.5681      0.2538      0.2008
        C      0.6258     -0.3273      0.3420
        O      0.4473     -0.2806      1.7299
        H     -0.2132     -0.9198      1.9728
        C     -0.5354      0.3617     -0.4431
        H     -0.3938      0.1980     -1.5380
        O     -1.7802     -0.2343     -0.2070
        H     -2.0886      0.0355      0.6509
        C     -0.5465      1.8605     -0.1348
        O     -1.3073      2.4738      0.5889
        O      0.4116      2.5760     -0.7639
        H      0.3497      3.4947     -0.5201
        C      0.8118     -1.7583     -0.1672
        O      0.4736     -2.7971      0.3667
        O      1.4416     -1.8587     -1.3585
        H      1.5142     -2.7728     -1.6160
        """)
