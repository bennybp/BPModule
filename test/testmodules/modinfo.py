from pulsar import OptionType

minfo = {

  "TestCppModule1" :
  {
    "type"        : "c_module",
    "base"        : "TestModule",
    "modpath"     : "testmodules.so",
    "version"     : "0.1a",
    "description" : "Simple C++ test module",
    "authors"     : ["Benjamin Pritchard"],
    "refs"        : [],
    "options"     : { }
  },
  
  "TestCppModule2" :
  {
    "type"        : "c_module",
    "base"        : "TestModule",
    "modpath"     : "testmodules.so",
    "version"     : "0.1a",
    "description" : "C++ module for testing exceptions",
    "authors"     : ["Benjamin Pritchard"],
    "refs"        : [],
    "options"     : { }
  },


 "TestPyModule1" :
  {
    "type"        : "python_module",
    "base"        : "TestModule",
    "version"     : "0.1a",
    "description" : "Simple python test module",
    "authors"     : ["Benjamin Pritchard"],
    "refs"        : [""],
    "options"     : { }
  },

 "TestPyModule2" :
  {
    "type"        : "python_module",
    "base"        : "TestModule",
    "version"     : "0.1a",
    "description" : "Python module for testing exceptions",
    "authors"     : ["Benjamin Pritchard"],
    "refs"        : [""],
    "options"     : { }
  },
}


