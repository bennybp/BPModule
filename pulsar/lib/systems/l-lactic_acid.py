import pulsar as psr
def load_ref_system():
    """ Returns l-lactic_acid as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        H      2.2684     -0.6369      0.3548
        C      1.6432     -0.7777     -0.5357
        H      1.8569      0.0482     -1.2279
        H      1.9664     -1.7066     -1.0238
        C      0.1651     -0.8465     -0.1594
        H     -0.0385     -1.7151      0.5097
        C     -0.2306      0.4454      0.5647
        O     -0.3088      0.6371      1.7630
        O     -0.5072      1.5092     -0.2240
        H     -0.7560      2.2561      0.3122
        O     -0.6699     -1.1217     -1.2524
        H     -0.5107     -0.4643     -1.9192
        """)
