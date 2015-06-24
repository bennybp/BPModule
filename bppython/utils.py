def PrintModuleInfo(key, minfo):

  print()
  print("  ++ Module: {}".format(key))
  print("            Name: {}".format(minfo["name"]))
  print("            Type: {}".format(minfo["type"]))
  print("            Path: {}".format(minfo["path"]))

  if "soname" in minfo:
      print("          SOName: {}".format(minfo["soname"]))

  print("         Version: {}".format(minfo["version"]))
  print("     Description: {}".format(minfo["description"]))

  print("         Authors: {}".format(len(minfo["authors"])))
  for aut in minfo["authors"]:
      print("                  {}".format(aut))

  print("      References: {}".format(len(minfo["refs"])))
  for ref in minfo["refs"]:
      print("                  {}".format(ref))

  print("         Options: {}".format(len(minfo["options"])))
  for opt in minfo["options"]:
      print("                  {:<12}  :  {:<12}  : {}".format(opt[0], opt[1], opt[2]))
  print()

