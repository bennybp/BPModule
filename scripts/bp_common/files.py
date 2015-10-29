#!/usr/bin/env python3

import os
import sys
import datetime


def GenIncludeGuard(path):
  base = os.path.basename(path)
  guard = base.upper()
  guard = guard.replace('.', '_')
  guard = guard.replace('-', '_')
  guard = guard.replace('/', '__')
  guard = guard.replace('\\', '__')
  guard = "_GUARD_{}_".format(guard)

  return guard




class HeaderSourceFiles:
    def __init__(self, base, desc, namespaces, hppincludes = [], cppincludes = [],
                 createheader = True, createsource = True):
        self.base = base
        self.desc = desc
        self.namespaces = namespaces
        self.hppincludes = hppincludes
        self.cppincludes = cppincludes
        self.cppfile = base + ".cpp" 
        self.hppfile = base + ".hpp"

        self.createheader = createheader
        self.createsource = createsource

        self.created = str(datetime.datetime.now())

        self.OpenHeaderSourcePair()
        

    def __enter__(self):
        return self


    def __exit__(self, exc_type, exc_val, exc_tb):
        self.CloseHeaderSourcePair()


    def OpenHeaderSourcePair(self):
        autogenstr = "/**************************************************************\n"
        autogenstr += "THIS FILE IS GENERATED VIA SCRIPTS. IF YOU WANT\n"
        autogenstr += "EVERYONE TO STAY SANE, DO NOT EDIT THIS FILE DIRECTLY\n\n"
        autogenstr += "Generated via:\n"
        autogenstr += "    " + ' '.join(sys.argv) + "\n"
        autogenstr += "\n"
        autogenstr += "CREATED: " + self.created + "\n"
        autogenstr += "**************************************************************/\n"

        self.namespaces.insert(0, "bpmodule")
        guard = GenIncludeGuard(self.hppfile)


        ##########################
        # SOURCE FILE ############
        ##########################
        if self.createsource:
            self.f = open(self.cppfile, 'w')
            self.f.write("/*\\file\n")
            self.f.write(" *\n")
            self.f.write(" * \\brief {} (source)\n".format(self.desc))
            self.f.write("*/\n")
            self.f.write("\n\n")
            self.f.write(autogenstr)
            self.f.write("\n\n")
            for inc in self.cppincludes:
                self.f.write("#include {}\n".format(inc)) 
            self.f.write("\n\n\n")

            for nm in self.namespaces:
                self.f.write("namespace {} {{\n".format(nm))

            self.f.write("\n\n\n")



        ##########################
        # HEADER FILE ############
        ##########################
        if self.createheader:
            self.fh = open(self.hppfile, 'w')
            self.fh.write("/*\\file\n")
            self.fh.write(" *\n")
            self.fh.write(" * \\brief {} (header)\n".format(self.desc))
            self.fh.write("*/\n")
            self.fh.write("\n\n")
            self.fh.write(autogenstr)
            self.fh.write("\n\n")
            self.fh.write("#ifndef {}\n".format(guard))
            self.fh.write("#define {}\n".format(guard))
            self.fh.write("\n\n")

            for inc in self.hppincludes:
                self.fh.write("#include {}\n".format(inc)) 
            self.fh.write("\n\n\n")

            for nm in self.namespaces:
                self.fh.write("namespace {} {{\n".format(nm))

            self.fh.write("\n\n\n")


    def CloseHeaderSourcePair(self):
        if self.createsource:
            self.f.write("\n\n\n")
            for nm in reversed(self.namespaces):
                self.f.write("}} // closing namespace {}\n".format(nm))

            self.f.close()


        if self.createheader:
            self.fh.write("\n\n\n")
            for nm in reversed(self.namespaces):
                self.fh.write("}} // closing namespace {}\n".format(nm))
        
            self.fh.write("\n\n\n")
            self.fh.write("#endif\n")

            self.fh.close()

