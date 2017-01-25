import pulsar as psr
def make_system():
    """ Returns the tentitive global minimum geometry of the water dimer
    according to the TIP5P potential.

    Ref:
	 T. James, D.J. Wales and J. Hernández-Rojas, 
            Chem. Phys. Lett., 415, 302 (2005)
    """
    return psr.make_system(""":
    O    -0.0638166135700000     1.3782116490000000     0.0000013991477170
    H     0.8583313934960812     1.6348726952004131    -0.0000192736448436
    H    -0.0465523195463249     0.4211673531851549     0.0000010357024237
    O     0.0596797744600000    -1.2953846140000000    -0.0000013030828870
    H    -0.3727778924225715    -1.6906360791564299     0.7569587260112379
    H    -0.3728118210906938    -1.6906361161127510    -0.7569419281358893
    """)
