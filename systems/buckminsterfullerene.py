import pulsar as psr
def load_ref_system():
    """ Returns buckminsterfullerene as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      0.0000      1.2363      3.3275
        C     -1.1758      0.3820      3.3275
        C     -0.7267     -1.0002      3.3275
        C      0.7267     -1.0002      3.3275
        C      1.1758      0.3820      3.3275
        C      2.3047      0.7489      2.5939
        C      3.0314     -0.2513      1.8298
        C      2.6002     -1.5785      1.8298
        C      1.4244     -1.9605      2.5939
        C      0.6977     -2.9607      1.8298
        C     -0.6977     -2.9607      1.8298
        C     -1.4244     -1.9605      2.5939
        C     -2.6002     -1.5785      1.8298
        C     -3.0314     -0.2513      1.8298
        C     -2.3047      0.7489      2.5939
        C     -2.3047      1.9852      1.8298
        C     -3.0314      1.7490      0.5935
        C     -3.4805      0.3668      0.5935
        C     -3.4805     -0.3668     -0.5935
        C     -3.0314     -1.7490     -0.5935
        C     -2.6002     -2.3426      0.5935
        C     -1.4244     -3.1968      0.5935
        C     -0.7267     -3.4235     -0.5935
        C      0.7267     -3.4235     -0.5935
        C      1.4244     -3.1968      0.5935
        C      2.6002     -2.3426      0.5935
        C      3.0314     -1.7490     -0.5935
        C      3.4805     -0.3668     -0.5935
        C      3.4805      0.3668      0.5935
        C      3.0314      1.7490      0.5935
        C      2.6002      2.3426     -0.5935
        C      2.6002      1.5785     -1.8298
        C      3.0314      0.2513     -1.8298
        C      2.3047     -0.7489     -2.5939
        C      2.3047     -1.9852     -1.8298
        C      1.1758     -2.8054     -1.8298
        C      0.0000     -2.4234     -2.5939
        C     -1.1758     -2.8054     -1.8298
        C     -2.3047     -1.9852     -1.8298
        C     -2.3047     -0.7489     -2.5939
        C     -3.0314      0.2513     -1.8298
        C     -2.6002      1.5785     -1.8298
        C     -2.6002      2.3426     -0.5935
        C     -1.4244      3.1968     -0.5935
        C     -0.6977      2.9607     -1.8298
        C     -1.4244      1.9605     -2.5939
        C     -0.7267      1.0002     -3.3275
        C     -1.1758     -0.3820     -3.3275
        C      0.0000     -1.2363     -3.3275
        C      1.1758     -0.3820     -3.3275
        C      0.7267      1.0002     -3.3275
        C      1.4244      1.9605     -2.5939
        C      0.6977      2.9607     -1.8298
        C      1.4244      3.1968     -0.5935
        C      0.7267      3.4235      0.5935
        C     -0.7267      3.4235      0.5935
        C     -1.1758      2.8054      1.8298
        C      0.0000      2.4234      2.5939
        C      1.1758      2.8054      1.8298
        C      2.3047      1.9852      1.8298
        """)
