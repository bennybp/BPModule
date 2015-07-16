
from bppython import Error, BPModuleException

def IsValidType(opt, typeallowed):
    valid = [ int, float, str, bool ]

    if typeallowed == True:
      # If just given a type, is it valid
      if opt in valid:
        return (True, None)

    # If actually given something, is it valid
    if type(opt) in valid:
      return (True, None)

    # if it's a list
    if type(opt) == list:
      if len(opt) == 0:
        return (False, "Is an empty list")

      # Given a type ie [ int ]
      if (typeallowed == True) and (type(opt[0]) == type):
        if len(opt) > 1: # given something like [bool, bool]
          return (False, "Is a list of types but has >1 elements")

        if not opt[0] in valid:
          return (False, "{} is not a valid type in a list\n".format(type(opt[0])))
        else:
          return (True, None)


      # Homogeneous?
      for v in opt:
        if type(v) != type(opt[0]):
          return (False, "Not a homogeneous list")

      # Is the list of valid types
      if not type(opt[0]) in valid:
          return (False, "{} is not a valid type in a list\n".format(type(opt[0])))

      # if we got here, everything is ok
      return (True, None)

    else:
        return (False, "{} not a valid type\n".format(type(opt)))




def IsValidDefaultTypes(defaults):
  ret = True
  for k,v in defaults.items():
    res,err = IsValidType(v[0], True)

    if not res:
      Error("Key %1%: %2%", k, err)
      ret = False

  return ret



def IsValidUserTypes(userset):
  ret = True
  for k,v in userset.items():
    res,err = IsValidType(v, False)

    if not res:
      Error("Key %1%: %2%", k, err)
      ret = False

  return ret



def CompareTypes(default, user):
    if type(default) == type:
        deftype = default
    else:
        deftype = type(default)

    # are the types not the same?
    if type(user) != deftype:
      return (False, "Type mismatch. Expected {}, but got {}".format(deftype, type(user)))
   
    # they are the same, but they might be lists
    # They have been checked for zero elements already 
    # (and for nested lists)
    if deftype == list:
      return CompareTypes(default[0], user[0])

    return (True, None)



def HasDefault(opt):
    # already checked for zero elements
    istype = (type(opt) == type) or (type(opt) == list and type(opt[0]) == type)
    return not istype



def MergeAndCheckOptions(defaults, userset):
    newopt = [ ]

    ret = True

    # Are all types valid for 
    # both defaults and userset
    if not IsValidDefaultTypes(defaults):
      raise BPModuleException("Developer error. Defaults don't all have a valid type! See above errors")


    # check for defaults for required options
    for k,v in defaults.items():
      if HasDefault(v[0]) and (v[1] == True):  # Required option, shouldn't have a default
        raise BPModuleException("Developer error. Required option {} given a default value".format(k))


    # Check the types passed in
    if not IsValidUserTypes(userset):
      ret = False


    # are all required keys specified
    for k,v in defaults.items():
        if v[1] == True and not k in userset:
            Error("Option \"%1%\" not specified, but is required\n", k)
            ret = False


    # Are all user keys in the default set?
    # also, compare the types
    for k,v in userset.items():
        if not k in defaults:
            Error("Option \"%1%\" not valid for this module\n", k)
            ret = False

        res, err = CompareTypes(defaults[k][0], v) 
        if not res:
            Error("Value for option \"%1%\" is not the correct type: %2%\n", k, err)
            ret = False


    if ret == False:
        raise BPModuleException("Error parsing options")


    # merge
    for k,v in defaults.items():
        if k in userset:
            newopt.append( (k, userset[k]) ) 
        elif HasDefault(v[0]):
            newopt.append( (k, v[0]) ) 

    # sort
    newopt.sort()


    # now run the check functions
    for v in newopt:
      k = v[0]
      checkfunc = defaults[k][2]
      if checkfunc != None:
        if checkfunc(k, v[1]) == False:
          Error("Validation failed for option \"%1%\"", k)
          ret = False 
        
    if ret == False:
        raise BPModuleException("Error parsing options")
 
    return newopt
    
