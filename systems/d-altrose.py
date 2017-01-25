import pulsar as psr
def load_ref_system():
    """ Returns d-altrose as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      3.1116     -0.6621      0.3109
        C      1.8893      0.2625      0.1071
        C      0.5528     -0.5144      0.2894
        C     -0.6746      0.4414      0.1256
        C     -2.0084     -0.3339      0.3301
        C     -3.1660      0.6606      0.4290
        O      4.3201      0.0390      0.3844
        O      1.9209      0.8891     -1.1625
        O      0.5839     -1.0666      1.5889
        O     -0.7287      1.0305     -1.1526
        O     -2.1996     -1.3527     -0.6139
        O     -4.1119      0.6562     -0.3198
        H      3.1586     -1.4356     -0.4830
        H      3.0670     -1.1782      1.2857
        H      1.9338      1.1371      0.7977
        H      0.4922     -1.3404     -0.4634
        H     -0.6130      1.2532      0.8940
        H     -1.9507     -0.9132      1.2871
        H     -3.0927      1.4019      1.2413
        H      4.3985      0.5678     -0.4004
        H      2.0482      0.2093     -1.8162
        H     -0.2668     -1.4731      1.7232
        H      0.1561      1.3460     -1.3432
        H     -2.3978     -0.9396     -1.4463
        """)
