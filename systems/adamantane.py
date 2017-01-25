import pulsar as psr
def load_ref_system():
    """ Returns adamantane as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C    0.8821211    0.8821211   -0.8821211
        C    1.7719176    0.0000000   -0.0000000
        C    0.8821211   -0.8821211    0.8821211
        H    2.4342233   -0.6124122   -0.6124122
        H    2.4342233    0.6124122    0.6124122
        C    0.0000000   -0.0000000    1.7719176
        C   -0.8821211    0.8821211    0.8821211
        H   -0.6124122   -0.6124122    2.4342233
        H    0.6124122    0.6124122    2.4342233
        C   -0.0000000    1.7719176    0.0000000
        H   -0.6124122    2.4342233   -0.6124122
        H    0.6124122    2.4342233    0.6124122
        C   -0.0000000    0.0000000   -1.7719176
        H    1.5107802    1.5107802   -1.5107802
        C    0.0000000   -1.7719176   -0.0000000
        H    1.5107802   -1.5107802    1.5107802
        C   -0.8821211   -0.8821211   -0.8821211
        H    0.6124122   -2.4342233   -0.6124122
        H   -0.6124122   -2.4342233    0.6124122
        H   -1.5107802   -1.5107802   -1.5107802
        C   -1.7719176   -0.0000000    0.0000000
        H   -0.6124122    0.6124122   -2.4342233
        H    0.6124122   -0.6124122   -2.4342233
        H   -2.4342233    0.6124122   -0.6124122
        H   -1.5107802    1.5107802    1.5107802
        H   -2.4342233   -0.6124122    0.6124122
        """)
