import pulsar as psr
def load_ref_system():
    """ Returns beta-d-mannopyranose as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C     -0.4820      1.3845     -0.8945
        O     -1.2325      0.5004     -0.0247
        C     -0.8609     -0.8899     -0.0294
        C      0.6151     -1.0100      0.4077
        O      1.0718     -2.3614      0.2503
        C      1.5151     -0.2196     -0.5667
        C      1.0542      1.2450     -0.6995
        O      1.3308      1.9691      0.5036
        O      2.8385     -0.1002     -0.0241
        C     -1.8527     -1.5043      0.9740
        O     -3.1161     -1.7022      0.3397
        O     -0.9196      2.6486     -0.5014
        H     -1.0168     -1.3119     -1.0480
        H      0.7684     -0.6735      1.4565
        H      1.5812     -0.7409     -1.5491
        H      1.5981      1.7558     -1.5291
        H     -0.8210      1.2472     -1.9401
        H     -0.6294      2.8427      0.4368
        H      2.2368      1.7262      0.8338
        H      3.1837     -0.9973      0.2027
        H      0.6927     -2.9419      0.9418
        H     -1.9774     -0.8508      1.8591
        H     -1.5618     -2.5251      1.2818
        H     -3.4866     -0.8291      0.0639
        """)
