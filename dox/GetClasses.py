#!/usr/bin/python3

import sys
import os
import re

bpmodule_root=os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
bpmodule_base=os.path.join(bpmodule_root,"bpmodule")

class_name=re.compile(r"\s*class\s+(?P<clsnm>\w+)\s*[\{:]\s+")
namespace_name=re.compile(r"\s*namespace\s+(?P<nsnm>\w+)\s*\{")


#List of classes that are in header files, but shouldn't be documented e.g.
#they are defined in a function
skip_class=["FDWrapper","WeightType"]

AllClasses={}
NS={}
for i in os.listdir(bpmodule_base):
   curr_dir=os.path.join(bpmodule_base,i)
   if os.path.isdir(curr_dir):
     AllClasses[i]=[]
     for j in os.listdir(curr_dir):
        curr_file=os.path.join(curr_dir,j)
        if j.endswith(".hpp"):
           f=open(curr_file,'r')
           ns=""
           for line in f:
              hit=re.search(class_name,line)
              if hit:
                 classname=hit.group('clsnm')
                 if classname not in skip_class:
                   AllClasses[i].append(classname)
                   NS[classname]=ns
              nm=re.search(namespace_name,line)
              if nm:
                 new_ns=nm.group('nsnm')
                 if new_ns == 'bpmodule':
                    ns=""
                 ns=ns+new_ns+str("::")
           f.close()

for key,val in AllClasses.items():
   f=open(os.path.join(bpmodule_root,"docs/source/"+str(key)+".rst"),"w")
   f.write("###############################################################\n")
   f.write(str(key)+" Core Components\n")
   f.write("###############################################################\n")
   f.write("\n.. toctree::\n")
   f.write("   :maxdepth:1\n")
   f.write("\n\n")
   for cl in val:
      f.write(".. autodoxyclass:: "+str(NS[cl])+str(cl)+"\n")
      f.write("   :members:\n\n")
   f.close()
