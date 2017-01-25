import pulsar as psr
def load_ref_system():
    """ Returns d-glutamine as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      1.5151     -0.7028     -1.6924
        C      0.5628     -1.0444     -0.5451
        C     -0.0726      0.2120      0.0615
        H      0.7006      0.9918      0.2065
        H     -0.8036      0.6561     -0.6450
        C     -0.7302     -0.1134      1.3934
        H     -1.5281     -0.8727      1.2577
        H      0.0139     -0.5769      2.0746
        C     -1.2879      1.1360      2.0434
        O     -0.7237      1.7638      2.9324
        N     -2.5317      1.6149      1.6216
        H     -2.9733      2.3583      2.1066
        H     -3.0661      1.0983      0.9670
        N     -0.4933     -1.9991     -0.9575
        O      1.2265      0.3827     -2.4452
        O      2.5137     -1.3131     -2.0286
        H      1.1747     -1.5580      0.2437
        H     -0.0807     -2.8370     -1.3026
        H     -1.0834     -1.5935     -1.6537
        H      1.9532      0.5532     -3.0378
        """)
