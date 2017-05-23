import pulsar as psr
def load_ref_system():
    """ Returns 2_2-dichloroacetic_acid as found in the IQMol fragment library.
        All credit to https://github.com/nutjunkie/IQmol
    """
    return psr.make_system("""
        C      0.1545     -0.2925     -0.4629
        C     -1.0813      0.5115     -0.1454
        O     -1.3397      1.1257      0.8694
       Cl      1.5716      0.7499     -0.3354
       Cl      0.3258     -1.5819      0.7287
        O     -2.0027      0.5337     -1.1357
        H      0.1163     -0.7381     -1.4838
        H     -2.7567      1.0535     -0.8754
        """)
