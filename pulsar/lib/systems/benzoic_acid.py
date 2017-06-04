import pulsar as psr
def load_ref_system():
    """ Returns benzoic_acid as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      2.2291     -0.1517      2.9720
        C      1.5782     -0.1115      2.0923
        C      0.1964     -0.0808      2.2505
        H     -0.2402     -0.0983      3.2547
        C     -0.6352     -0.0273      1.1374
        H     -1.7228     -0.0023      1.2721
        C     -0.0788     -0.0060     -0.1439
        C     -0.9318      0.0432     -1.3534
        O     -0.6482     -0.2725     -2.4972
        O     -2.1954      0.5046     -1.1851
        H     -2.6479      0.4922     -2.0230
        C      1.3094     -0.0420     -0.3022
        H      1.7464     -0.0340     -1.3085
        C      2.1329     -0.0923      0.8167
        H      3.2205     -0.1183      0.6913
        """)
