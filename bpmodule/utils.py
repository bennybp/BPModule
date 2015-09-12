from bpmodule import output

def PrintModuleInfo(minfo):

  output.Output("\n")
  output.Output("  ++ Module: %1%\n", minfo["name"])
  output.Output("             Key: %1%\n", minfo["key"])
  output.Output("            Type: %1%\n", minfo["type"])
  output.Output("            Path: %1%\n", minfo["path"])

  if "soname" in minfo:
      output.Output("          SOName: %1%\n", minfo["soname"])

  output.Output("         Version: %1%\n", minfo["version"])
  output.Output("     Description: %1%\n", minfo["description"])

  output.Output("         Authors: %1%\n", len(minfo["authors"]))
  for aut in minfo["authors"]:
      output.Output("                  %1%\n", aut)

  output.Output("      References: %1%\n", len(minfo["refs"]))
  for ref in minfo["refs"]:
      output.Output("                  %1%\n", ref)

  if "passedoptions" in minfo:
      nopt = len(minfo["passedoptions"])
      output.Output("         Options: %1%\n", nopt)

      if nopt > 0:
          output.Output("\n")
          l = "-"*20
          output.Output("      %|1$-20|      %|2$-20|      %|3$-20|     %4%\n", "Option", "Default", "Value", "Description")
          output.Output("      %|1$-20|      %|2$-20|      %|3$-20|     %4%\n", l, l, l, l)

          defopt = minfo["options"]
          for opt in minfo["passedoptions"]:
              d = defopt[opt[0]]
              help = d[3]

              if type(d[0]) == type:
                dv = "({})".format(d[0].__name__)
              elif type(d[0]) == list and type(d[0][0]) == type:
                dv = "[ ({}) ]".format(d[0][0].__name__)
              else:
                dv = d[0]

              # if not the same, highlight change
              if dv != opt[1]:
                  OutFunc = output.Changed
              else:
                  OutFunc = output.Output
      
              if type(opt[1]) == list:  # note - zero elements not allowed
                if type(dv) == list:
                  nel = max(len(opt[1]), len(dv))
                else:
                  nel = len(opt[1])


                # first row
                if type(dv) == list:
                  OutFunc("      %|1$-20|      %|2$-20|      %|3$-20|     %4%\n", opt[0], dv[0], opt[1][0], help)
                else: 
                  OutFunc("      %|1$-20|      %|2$-20|      %|3$-20|     %4%\n", opt[0], dv, opt[1][0], help)

                for i in range(1, nel):
                  dstr = dv[i] if (type(dv) == list and i < len(dv)) else ""
                  vstr = opt[1][i] if (i < len(opt[1])) else ""

                  OutFunc("      %|1$-20|      %|2$-20|      %|3$-20|     %4%\n", "", dstr, vstr, "")
        
              else:        
                OutFunc("      %|1$-20|      %|2$-20|      %|3$-20|     %4%\n", opt[0], dv, opt[1], help)

  else:
      output.Output("         Options: NOT YET SET\n")

  output.Output("\n")

