import pulsar as psr
def load_ref_system():
    """ Returns d-threonine as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        O      2.0867     -0.4359      1.2186
        C      0.9764      0.2795      0.7387
        C      1.3815      1.6731      0.2597
        C      0.2209     -0.4763     -0.3959
        N      0.1417     -1.9295     -0.1257
        C     -1.1441      0.1740     -0.6080
        O     -2.0808     -0.0875      0.3338
        O     -1.5042      0.8873     -1.5253
        H      2.6531     -0.6205      0.4792
        H      0.3422      0.3456      1.6537
        H      2.0494      1.6317     -0.6119
        H      1.9087      2.2241      1.0495
        H      0.5022      2.2644     -0.0257
        H      0.7981     -0.3797     -1.3515
        H     -0.5133     -2.3532     -0.7470
        H     -0.1289     -2.0990      0.8198
        H     -2.8940      0.3582      0.1164
        """)
