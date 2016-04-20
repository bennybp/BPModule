import bpmodule as bp
import numpy as np

def CoI():
    """Returns the matrix representation for a center of inversion
    
    
    Upon inverting through the origin each point 
      \f$\left\lbrace x,y,z\right\rbrace\f$ becomes
      \f$\left\lbrace -x,-y,-z\right\rbrace\f$.  Hence the matrix representation
    of inversion through the origin is simply
    /f[
       \begin{bmatrix}
       -1 &  0 &  0\\
        0 & -1 &  0\\
        0 &  0 & -1
       \end{bmatrix}
    /f]
    
    Return:
       The matrix representation for inversion through the origin
    """
    return np.array([[-1,0,0],[0,-1,0],[1])


class Symmetrizer:
    def GetSymmetry(self,Mol,Tol):
        """Determines the symmetries that a molecule is closest to
        
        Args:
        
           Mol (system) : The system you want the symmetry of
           
           Tol (double) : To what precision two atoms must lie on top of one
                          another to be the same
        """
        CoM=Mol.CenterOfMass()
        CenteredMol=Mol.Translate([-1*i for i in CoM])
        I=np.array(bp.system.InertiaTensor(CenteredMol)).reshape(3,3)
        evals,evecs=np.linalg.eig(I)
        #Sort eigenvalues in descending order
        idx=evals.argsort()[::-1]
        evals=evals[idx]
        evecs=evecs[:,idx].transpose()
        
        #Align molecule
        InertiadMol=CenteredMol.Rotate(evecs.flatten().tolist())
        