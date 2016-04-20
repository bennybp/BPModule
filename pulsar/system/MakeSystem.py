import re
import bpmodule as bp
from bpmodule.exception import GeneralException

def MakeSystem(SomeString):
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
    * The following are currently supported in Psi4, but presently ignored:
      * 'no_reorient' or 'noreorient' request that molecule not be reoriented
      * 'no_com' or 'nocom' request that molecule not be translated to center of mass
      * 'symmetry X' or 'symmetry=X' requests that molecule has symmetry 'X'
      * Z-Mat currently not supported
    """
    #For the below comments, "any number" includes 0
   
    #Matches a comment line
    comment = re.compile(r'^\s*#')
    
    #Matches a line that is blank aside from white-space
    blank = re.compile(r'^\s*$')
    
    #Matches a request for atomic units
    bohr = re.compile(r'^\s*units?[\s=]+(bohr|au|a.u.)\s*$', re.IGNORECASE)
    
    #Matches a request for Angstroms
    ang = re.compile(r'^\s*units?[\s=]+(ang|angstrom)\s*$', re.IGNORECASE)
    
    #Matches a line requesting no reorientation
    #orient = re.compile(r'^\s*(no_reorient|noreorient)\s*$', re.IGNORECASE)
    
    #Matches a line requesting no shift to the center of mass
    #com = re.compile(r'^\s*(no_com|nocom)\s*$', re.IGNORECASE)
    
    #Matches a request setting the symmetry
    #symmetry = re.compile(r'^\s*symmetry[\s=]+(\w+)\s*$', re.IGNORECASE)
    
    #Matches 1-3 letters followed by an underscore, followed by any number of
    #characters or 1-3 letters followed by any number of numbers
    #ATOM = '((([A-Z]{1,3})_\w+)|(([A-Z]{1,3})\d*))'
    
    #Ghosts and atoms?
    atom = re.compile(r'^(?:(?P<gh1>@)|(?P<gh2>Gh\())?(?P<label>(?P<symbol>[A-Z]{1,3})(?:(_\w+)|(\d+))?)(?(gh2)\))(?:@(?P<mass>\d+\.\d+))?$', re.IGNORECASE)
    
    #Charge and multiplicity
    cgmp = re.compile(r'^\s*(-?\d+)\s+(\d+)\s*$')
    
    #Looks for '--'
    frag = re.compile(r'^\s*--\s*$')

    #Matches something equals a number
    variable = re.compile(r'^\s*(\w+)\s*=\s*(-?\d+\.\d+|-?\d+\.|-?\.\d+|-?\d+|tda)\s*$', re.IGNORECASE)
    
    #Matches any line that starts with '@ ' or 'Gh(X)'
    ghost = re.compile(r'@(.*)|Gh\((.*)\)', re.IGNORECASE)


    DaAtoms={"SYSTEM":[]} #Atoms per fragment, SYSTEM is all atoms
    ToAU=1/0.52917721067
    Charge=[0] #Charges, 0-th element is full system, i-th (i>0) is i-th frag
    Mult=[1] #Multiplicities, same as charges

    lines = re.split('\n', SomeString)
    for line in lines:
        #Skip comments and blank lines
        if comment.match(line) or blank.match(line) or ang.match(line):
            continue

        elif bohr.match(line): #Is bohr
            ToAU=1.0

        # handle charge and multiplicity
        elif cgmp.match(line):
            Charge[len(DaAtoms)-1] = int(cgmp.match(line).group(1))
            Mult[len(DaAtoms)-1] = int(cgmp.match(line).group(2))

        # handle fragment markers and default fragment cgmp
        elif frag.match(line):
            Charge.append(0)
            Mult.append(1)
            DaAtoms[len(DaAtoms)]=[]

        elif atom.match(line.split()[0].strip()):
                entries = re.split(r'\s+|\s*,\s*', line.strip())
                atomm = atom.match(line.split()[0].strip().upper())
                atomLabel = atomm.group('label')
                atomSym = atomm.group('symbol')
                
                # We don't know whether the @C or Gh(C) notation matched. Do a quick check.
                ghostAtom = False if (atomm.group('gh1') is None and atomm.group('gh2') is None) else True

                # handle cartesians
                Carts=[]
                if len(entries) == 4:
                    Carts.append(ToAU*float(entries[1]))
                    Carts.append(ToAU*float(entries[2]))
                    Carts.append(ToAU*float(entries[3]))
                
                TempAtom=bp.system.CreateAtom(len(DaAtoms["SYSTEM"]),Carts,bp.system.AtomicZNumberFromSym(atomSym))
                DaAtoms["SYSTEM"].append(TempAtom)
                if len(DaAtoms) > 1:
                   DaAtoms[len(DaAtoms)-1].append(TempAtom)
        else:
            raise GeneralException('MakeSystem: Unidentifiable line in geometry specification: %s' % (line))

    molu=bp.system.AtomSetUniverse()
    for i in DaAtoms["SYSTEM"]:
        molu.Insert(i)
    DaSys=bp.system.System(molu,True)
    #DaSys.SetCharge(Charge[0])
    #DaSys.SetMultiplicity(Mult[0])
    return DaSys


