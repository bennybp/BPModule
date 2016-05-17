import re
import math
import pulsar as psr
from pulsar.exception import GeneralException

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
    * Unit cells of crystals may be used as input.  This is done by providing
      the fractional coordinates of your atoms and the cell's dimensions
      * sides are specified by 'sides a b c'
        * Units can be specified by units keyword
      * angles are specified by 'angles alpha beta gamma'
        * Units are in degrees
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

    #Mathches a line that starts with 'sides' and has three numbers
    UCsides = re.compile(r'^\s*sides\s*(\d+|\d+\.\d+)\s*(\d+|\d+\.\d+)\s*(\d+|\d+\.\d+)\s*$',re.IGNORECASE)

    #Mathches a line that starts with 'sides' and has three numbers
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
        #Skip comments and blank lines
        if comment.match(line) or blank.match(line) or ang.match(line):
            continue

        elif bohr.match(line): #Is bohr
            ToAU=1.0

        # handle charge and multiplicity
        elif cgmp.match(line):
            Charge[NFrags()] = int(cgmp.match(line).group(1))
            Mult[NFrags()] = int(cgmp.match(line).group(2))

        # handle fragment markers and default fragment cgmp
        elif frag.match(line):
            Systems.append(0)
            Charge.append(0)
            Mult.append(1)
            DaAtoms[len(DaAtoms)]=[]
        
        # handle UC stuff
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
                Zs.append(psr.system.AtomicZNumberFromSym(atomSym))
                Systems[NFrags()]+=1
                
        else:
            raise GeneralException('MakeSystem: Unidentifiable line in geometry specification: %s' % (line))

    DaSpace=psr.system.Space()
    Periodic=(len(Sides)==3 and len(Angles)==3)
    NewSides=[ToAU*i for i in Sides]
    if Periodic:
        DaSpace=psr.system.Space(Angles,NewSides)
        ToAU=1.0
    molu=psr.system.AtomSetUniverse()
    for i in range(0,len(Zs)):
        TempCarts=[ToAU*Carts[3*i+j] for j in range(0,3)]
        molu.Insert(psr.system.CreateAtom(i,TempCarts,Zs[i]))
    DaSys=psr.system.System(molu,True)
    DaSys.SetSpace(DaSpace)
    DaSys.SetCharge(Charge[0])
    DaSys.SetMultiplicity(Mult[0])
    if Periodic:
        CosA=[math.cos(math.radians(i)) for i in Angles]
        CosBG=CosA[1]*CosA[2]
        SinG=math.sin(math.radians(Angles[2]))
        v=math.sqrt(1-CosA[0]**2-CosA[1]**2-CosA[2]**2-2*CosA[0]*CosBG)
        molu=psr.system.AtomSetUniverse()
        Temp=DaSys.Rotate([NewSides[0],NewSides[1]*CosA[2],NewSides[2]*CosA[1],
                     0,NewSides[1]*SinG,NewSides[2]*(CosA[0]-CosBG)/SinG,
                     0,0,NewSides[2]*v/SinG])
        for x in range(0,3):
            for y in range(0,3):
                for z in range(0,3,1):
                    Temp2=Temp.Translate([(x-1)*NewSides[0],
                                          (y-1)*NewSides[1],
                                          (z-1)*NewSides[2]])
                    for i in Temp2:
                        Diff=[math.fabs(i[j]-.1*NewSides[j]) for j in range(0,3)]
                        for j in range(0,3):
                            if Diff[j]<NewSides[j] or math.fabs(i[j])<.1:
                              if j==2 and not molu.Contains(i):
                                 molu.Insert(i)
                            else:
                              break
                                  
        DaSys=psr.system.System(molu,True)
    return DaSys


