The Wavefunction Class in Pulsar                                 {#wavefunction}
================================

In quantum mechanics the wavefunction is a mathematical object that contains all
the knowable information about a molecular system.  The wavefunction in Pulsar
tries to adhere to this as well.

For convenience we define the following quantities for this page:
- \f$n\f$ the number of spatial atomic orbitals (AO)
- \f$N\f$ the number of spatial molecular orbitals (MO)


- The system
  - See [here](@ref systems) for more information on using this class
- The molecular orbital coefficients
  - A \f$n\f$ by \f$N\f$ matrix \f$\mathbf{C}\f$ such that \f$C_{\mu i}\f$ is
    the weight of the \f$\mu\f$-th AO in the linear expansion of the \f$i\f$-th
    MO
- The one particle density matrix (in the AO basis)
  - Formed by \f$CC^T\f$ it is an \f$n\f$ by \f$n\f$ matrix \f$\mathbf{P}\f$
    such that \f$P_{ij}\f$ is the degree to which AO \f$i\f$ and \f$j\f$ are
    coupled (also called the coherence between the states)
- The molecular orbital eigenvalues (the Fock matrix in the MO basis)
  - A \f$N\f$ element vector \$\vec{\epsilon}\f$ such that \f$\epsilon_i\f$ is
    the energy of the \f$i\f$-th MO in Hartrees
- The molcular orbital occupations (\f$\mathbf{P}\f$ in the MO basis)
  - A \f$N\f$ element vector such that the \f$i\f$-th element is the occupation
    of the \f$i\f$-th MO

All of the aforementioned matrices and vectors are stored as IrrepSpinTensor
instances, which means you have one of the above matrices or vectors for each
spin irrep pair; however, the dimensions of the arrays change in a way that
needs documented when I understand it...
