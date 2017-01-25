import pulsar as psr
def load_ref_system():
    """ Returns benzaldehyde as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      0.3179      1.0449     -0.0067
        C      1.6965      0.8596     -0.0102
        C      2.2283     -0.4253     -0.0050
        C      1.3808     -1.5297      0.0037
        C      0.0035     -1.3492      0.0073
        C     -0.5347     -0.0596      0.0021
        C     -2.0103      0.0989      0.0061
        O     -2.5724      1.1709      0.0021
        H      2.3631      1.7283     -0.0171
        H      3.3139     -0.5693     -0.0078
        H      1.8000     -2.5413      0.0078
        H     -0.6626     -2.2203      0.0142
        H     -2.6021     -0.8324      0.0131
        H     -0.1030      2.0579     -0.0108
        """)
