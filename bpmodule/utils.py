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

  nopt = len(minfo["options"])
  output.Output("         Options: %1%\n", nopt)

  if nopt > 0:
      output.Output("\n")
      l = "-"*20
      l2 = "-"*10
      output.Output("      %|1$-20|      %|2$-20|      %|3$-20|     %|4$-10|       %5%\n", "Option", "Type", "Default", "Required", "Description")
      output.Output("      %|1$-20|      %|2$-20|      %|3$-20|     %|4$-10|       %5%\n", l, l, l, l2, l)

      for key,value in minfo["options"].items():
            val = value[1] 
            if type(val) == list:
                # first row
                output.Output("      %|1$-20|      %|2$-20|      %|3$-20|     %|4$-10|       %5%\n", key, value[0], value[1][0], value[2], value[4])

                # other rows
                for i in range(1, len(value[1])):
                    output.Output("      %|1$-20|      %|2$-20|      %|3$-20|     %|4$-10|       %5%\n", "", "", value[1][i], "", "")

            else: 
              output.Output("      %|1$-20|      %|2$-20|      %|3$-20|     %|4$-10|       %5%\n", key, value[0], value[1], value[2], value[4])

  output.Output("\n")

