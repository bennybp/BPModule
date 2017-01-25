import pulsar as psr
def load_ref_system():
    """ Returns n_n-dimethylacetamide as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      1.2796      1.4165      0.0945
        N      0.6776      0.0700      0.2226
        C      1.5140     -1.0082     -0.3487
        C     -0.7486     -0.0450      0.0569
        C     -1.3639     -1.4177     -0.0335
        O     -1.4170      0.9773      0.0668
        H      0.7287      2.1344      0.7165
        H      1.2794      1.7842     -0.9418
        H      2.3174      1.3826      0.4520
        H      1.4657     -1.0502     -1.4470
        H      1.1955     -1.9809      0.0498
        H      2.5619     -0.8539     -0.0576
        H     -1.0501     -1.9328     -0.9519
        H     -2.4610     -1.3621     -0.0456
        H     -1.0758     -2.0443      0.8216
        """)
