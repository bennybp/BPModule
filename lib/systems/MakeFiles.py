import os
for root, dirs, files in os.walk("Molecules"):
    for name in files:
        if name.endswith(".xyz"):
            mol_name=os.path.splitext(name)[0].lower()
            f=open(mol_name+".py","w")
            f.write("import pulsar as psr\n")
            f.write("def load_ref_system():\n")
            f.write("    \"\"\" Returns "+mol_name+" as found in the IQMol fragment library.\n")
            f.write("        All credit to https://github.com/nutjunkie/IQmol\n")
            f.write("    \"\"\"\n")
            f.write("    return psr.make_system(\"\"\"\n")
            print(os.path.join(root,name))
            with open(os.path.join(root,name)) as g:
                next(g) #Number of atoms
                next(g) #Comment line
                for line in g:
                    f.write("      "+line)
            
            f.write("        \"\"\")\n")
            f.close()
           
