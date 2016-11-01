import re
import math
from copy import deepcopy
import pulsar as psr

def make_system(SomeString):
    """This function turns a string into a system object, which it then returns

    Special thanks to Lori A. Burns for the original version of this function

    =================================
    Rules For Structuring Your String
    =================================
    * An entire line may be commented out by prepending a '#' character
    * To specify the units of your system:
      * Add the line: 'units X' or 'units=X'
      * 'X' may be: 
        * 'bohr', 'au', or 'a.u.' for atomic units
        * 'ang'or angstrom for Angstroms
    * Unit cells of crystals may be used as input.  This is done by providing
      the fractional coordinates of your atoms and the cell's dimensions
      * sides are specified by 'sides a b c'
        * Units can be specified by units keyword
      * angles are specified by 'angles alpha beta gamma'
        * Units are in degrees
    """
    #For the below comments, "any number" includes 0
    comment = re.compile(r'^\s*#')#Comment line
    blank = re.compile(r'^\s*$')#Blank aside from white-space
    bohr = re.compile(r'^\s*units?[\s=]+(bohr|au|a.u.)\s*$', re.IGNORECASE)#a.u.
    ang = re.compile(r'^\s*units?[\s=]+(ang|angstrom)\s*$', re.IGNORECASE)#Ang
    #Ghosts and atoms?
    atom = re.compile(r'^(?:(?P<gh1>@)|(?P<gh2>Gh\())?(?P<label>(?P<symbol>[A-Z]{1,3})(?:(_\w+)|(\d+))?)(?(gh2)\))(?:@(?P<mass>\d+\.\d+))?$', re.IGNORECASE)
    cgmp = re.compile(r'^\s*(-?\d+)\s+(\d+)\s*$')#Charge/Mult
    frag = re.compile(r'^\s*--\s*$')#Fragment sperator
    #Matches something equals a number
    variable = re.compile(r'^\s*(\w+)\s*=\s*(-?\d+\.\d+|-?\d+\.|-?\.\d+|-?\d+|tda)\s*$', re.IGNORECASE)
    ghost = re.compile(r'@(.*)|Gh\((.*)\)', re.IGNORECASE)#Matches ghosts

    #Mathches a line that starts with 'sides' and has three numbers
    UCsides = re.compile(r'^\s*sides\s*(\d+|\d+\.\d+)\s*(\d+|\d+\.\d+)\s*(\d+|\d+\.\d+)\s*$',re.IGNORECASE)

    #Mathches a line that starts with 'angles' and has three numbers
    UCangles = re.compile(r'^\s*angles\s*(\d+|\d+\.\d+)\s*(\d+|\d+\.\d+)\s*(\d+|\d+\.\d+)\s*$',re.IGNORECASE)

    Systems=[0] #Atoms per fragment
    Zs=[] #Atomic number of each atom
    ToAU=1/0.52917721067
    Charge=[0] #Charges, 0-th element is full system, i-th (i>0) is i-th frag
    Mult=[1] #Multiplicities, same as charges
    Sides=[]
    Angles=[]
    Carts=[]
    
    def NFrags():
        return len(Systems)-1

    lines = re.split('\n', SomeString)
    for line in lines:
        if comment.match(line) or blank.match(line) or ang.match(line):
            continue

        elif bohr.match(line):
            ToAU=1.0
            
        elif cgmp.match(line):
            Charge[NFrags()] = int(cgmp.match(line).group(1))
            Mult[NFrags()] = int(cgmp.match(line).group(2))

        # handle fragment markers and default fragment cgmp
        elif frag.match(line):
            Systems.append(0)
            Charge.append(0)
            Mult.append(1)
            DaAtoms[len(DaAtoms)]=[]

        elif UCsides.match(line):
            for i in range(1,4):
                Sides.append(float(UCsides.match(line).group(i)))
                
        elif UCangles.match(line):
            for i in range(1,4):
                Angles.append(float(UCangles.match(line).group(i)))

        # handle atoms
        elif atom.match(line.split()[0].strip()):
                entries = re.split(r'\s+|\s*,\s*', line.strip())
                atomm = atom.match(line.split()[0].strip().upper())
                atomLabel = atomm.group('label')
                atomSym = atomm.group('symbol')
                
                # We don't know whether the @C or Gh(C) notation matched. Do a quick check.
                ghostAtom = False if (atomm.group('gh1') is None and atomm.group('gh2') is None) else True

                # handle cartesians
                if len(entries) == 4:
                    for i in range(1,4):
                        Carts.append(float(entries[i]))
                Zs.append(psr.atomic_z_from_symbol(atomSym))
                Systems[NFrags()]+=1
                
        else:
            raise GeneralException('make_system: Unidentifiable line in geometry specification: %s' % (line))

    DaSpace=psr.Space()
    Periodic=(len(Sides)==3 and len(Angles)==3)
    NewSides=[ToAU*i for i in Sides]
    if Periodic:
        DaSpace=psr.Space(Angles,NewSides)
        ToAU=1.0
    molu=psr.AtomSetUniverse()
    for i in range(0,len(Zs)):
        TempCarts=[ToAU*Carts[3*i+j] for j in range(0,3)]
        molu.insert(psr.create_atom(TempCarts,Zs[i]))
    DaSys=psr.System(molu,True)
    if Periodic:
        Newu=psr.system.Frac2Cart(molu,DaSpace)
        UC=psr.CarveUC(
            psr.MakeSuperCell(Newu,[3,3,3],DaSpace.LatticeSides),
            DaSpace.LatticeSides)
        molu=psr.CleanUC(UC,DaSpace.LatticeSides)
    DaSys=psr.System(molu,True)
    DaSys.space=DaSpace
    DaSys.charge=Charge[0]
    DaSys.multiplicity=Mult[0]
    return DaSys


