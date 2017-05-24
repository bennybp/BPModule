import pulsar as psr
def load_ref_system():
    """ Returns di-tert-butylether as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      0.9109     -0.1733      0.8561
        C      2.3814     -0.6170      0.7353
        H      2.4636     -1.6576      0.3950
        H      2.9374      0.0083      0.0243
        H      2.8876     -0.5444      1.7067
        C      0.1967     -1.1084      1.8403
        H      0.0935     -2.1222      1.4320
        H      0.7570     -1.1824      2.7812
        H     -0.8142     -0.7446      2.0851
        C      0.8734      1.2598      1.3933
        H      1.3148      1.3069      2.3968
        H      1.4374      1.9461      0.7473
        H     -0.1566      1.6460      1.4616
        O      0.4370     -0.3183     -0.4900
        C     -0.8556      0.1331     -0.9179
        C     -2.0262     -0.4184     -0.0998
        H     -2.0257     -1.5167     -0.0866
        H     -1.9915     -0.0781      0.9479
        H     -2.9827     -0.0899     -0.5258
        C     -0.9558      1.6621     -0.9896
        H     -1.0550      2.1098      0.0123
        H     -0.0691      2.1035     -1.4628
        H     -1.8333      1.9662     -1.5745
        C     -0.9435     -0.4329     -2.3482
        H     -0.1486     -0.0331     -2.9915
        H     -0.8574     -1.5276     -2.3580
        H     -1.9055     -0.1721     -2.8081
        """)