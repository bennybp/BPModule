from pulsar import OptionType

minfo = {

  "TestCppModule1" :
  {
    "type"        : "c_module",
    "base"        : "TestModule",
    "modpath"     : "testmodules.so",
    "version"     : "0.1a",
    "description" : "Simple C++ test module",
    "authors"     : ["Pulsar Core Developers"],
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
    "authors"     : ["Pulsar Core Developers"],
    "refs"        : [],
    "options"     : { }
  },

  "TestCppModule3" :
  {
    "type"        : "c_module",
    "base"        : "TestModule",
    "modpath"     : "testmodules.so",
    "version"     : "0.1a",
    "description" : "C++ module for testing nested module calls",
    "authors"     : ["Pulsar Core Developers"],
    "refs"        : [],
    "options"     : {
                        "OTHER_MODULE" : ( OptionType.String, None, False, None, "Other module to call") 
                    }
  },


 "TestPyModule1" :
  {
    "type"        : "python_module",
    "base"        : "TestModule",
    "version"     : "0.1a",
    "description" : "Simple python test module",
    "authors"     : ["Pulsar Core Developers"],
    "refs"        : [""],
    "options"     : { }
  },

 "TestPyModule2" :
  {
    "type"        : "python_module",
    "base"        : "TestModule",
    "version"     : "0.1a",
    "description" : "Python module for testing exceptions",
    "authors"     : ["Pulsar Core Developers"],
    "refs"        : [""],
    "options"     : { }
  },

 "TestPyModule3" :
  {
    "type"        : "python_module",
    "base"        : "TestModule",
    "version"     : "0.1a",
    "description" : "Python module for testing nested module calls",
    "authors"     : ["Pulsar Core Developers"],
    "refs"        : [""],
    "options"     : {
                        "OTHER_MODULE" : ( OptionType.String, None, False, None, "Other module to call") 
                    }
  },
}


