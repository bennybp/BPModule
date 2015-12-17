from bpmodule.datastore import PythonType

minfo = {

  "TestModule1" :
  {
    "type"        : "c_module",
    "soname"      : "TestModules.so",
    "version"     : "0.1a",
    "description" : "Some test module",
    "authors"     : ["me", "myself", "I"],
    "refs"        : ["some paper", "some other paper"],
    "options"     : {
                        # Key                 Type    Default     Req     Check   Help
                        "bool_opt_def":    (  PythonType.Bool,  False,     False,   None,  "Some help string"),
                        "bool_opt":        (  PythonType.Bool,  None,      False,   None,  "Some help string"),

                        "int_opt_def":     (  PythonType.Int,    1,        False,   None,  "Some help string"),
                        "int_opt":         (  PythonType.Int,    None,     False,   None,  "Some help string"),

                        "double_opt_def":  ( PythonType.Float,   6.02e23,  False,   None,  "Some help string"),
                        "double_opt":      ( PythonType.Float,   None,     False,   None,  "Some help string"),

                        "str_opt_def":     (  PythonType.String,    "Hi",     False,   None,  "Some help string"),
                        "str_opt":         (  PythonType.String,    None,     False,   None,  "Some help string"),
                    }
  },


 "TestPyModule1" :
  {
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Some test module",
    "authors"     : ["me", "myself", "I"],
    "refs"        : ["some paper", "some other paper"],
    "options"     : {
                        # Key                 Type    Default     Req     Check   Help
                        "bool_opt_def":    (  PythonType.Bool,  False,     False,   None,  "Some help string"),
                        "bool_opt":        (  PythonType.Bool,  None,      False,   None,  "Some help string"),

                        "int_opt_def":     (  PythonType.Int,    1,        False,   None,  "Some help string"),
                        "int_opt":         (  PythonType.Int,    None,     False,   None,  "Some help string"),

                        "double_opt_def":  ( PythonType.Float,   6.02e23,  False,   None,  "Some help string"),
                        "double_opt":      ( PythonType.Float,   None,     False,   None,  "Some help string"),

                        "str_opt_def":     (  PythonType.String,    "Hi",     False,   None,  "Some help string"),
                        "str_opt":         (  PythonType.String,    None,     False,   None,  "Some help string"),
                    }
  },


  ##########################################
  # Testing of linking to external libraries
  ##########################################
  "TestExtLib" :
  {
    "type"        : "c_module",
    "soname"      : "TestModules.so",
    "version"     : "0.1a",
    "description" : "Some test module",
    "authors"     : ["me", "myself", "I"],
    "refs"        : ["some paper", "some other paper"],
    "options"     : {
                        # Key                 Type    Default     Req     Check   Help
                    }
  },



  ##########################################
  # Testing of options parsing. These modules
  # aren't meant to be loaded
  ##########################################
  "TestOptions_int" :
  {
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of integers",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        # Key                 Type    Default     Req     Check   Help
                        "int_opt_def":     (  PythonType.Int,   5,         False,   None,  "Some help string"),
                        "int_req":         (  PythonType.Int,   None,      True,    None,  "Some help string"),
                        "int_opt":         (  PythonType.Int,   None,      False,   None,  "Some help string"),
                    }
  },

  "TestOptions_float" :
  {
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of floats/doubles",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        # Key                   Type    Default     Req     Check   Help
                        "float_opt_def":    (  PythonType.Float,  5.0,       False,   None,  "Some help string"),
                        "float_req":        (  PythonType.Float,  None,      True,    None,  "Some help string"),
                        "float_opt":        (  PythonType.Float,  None,      False,   None,  "Some help string"),
                    }
  },

  "TestOptions_bool" :
  {
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of bools",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        # Key                 Type    Default     Req     Check   Help
                        "bool_opt_def":    (  PythonType.Bool,  False,     False,   None,  "Some help string"),
                        "bool_req":        (  PythonType.Bool,  None,      True,    None,  "Some help string"),
                        "bool_opt":        (  PythonType.Bool,  None,      False,   None,  "Some help string"),
                    }
  },

  "TestOptions_str" :
  {
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of strings",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        # Key                 Type    Default     Req     Check   Help
                        "str_opt_def":     (  PythonType.String,   "Hello",   False,   None,  "Some help string"),
                        "str_req":         (  PythonType.String,   None,      True,    None,  "Some help string"),
                        "str_opt":         (  PythonType.String,   None,      False,   None,  "Some help string"),
                    }
  },

  "TestOptions_listint" :
  {
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of list of integers",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        # Key                 Type         Default         Req     Check   Help
                        "listint_opt_def":  (  PythonType.ListInt,   [ 1, 2, 3 ],   False,   None,  "Some help string"),
                        "listint_req":      (  PythonType.ListInt,   None,          True,    None,  "Some help string"),
                        "listint_opt":      (  PythonType.ListInt,   None,          False,   None,  "Some help string"),
                    }
  },

  "TestOptions_listfloat" :
  {
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of list of floats/doubles",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        # Key                   Type         Default                Req      Check   Help
                        "listfloat_opt_def":  (  PythonType.ListFloat,   [ 1.0, 2.0, 3.0 ],   False,   None,  "Some help string"),
                        "listfloat_req":      (  PythonType.ListFloat,   None,                True,    None,  "Some help string"),
                        "listfloat_opt":      (  PythonType.ListFloat,   None,                False,   None,  "Some help string"),
                    }
  },

  "TestOptions_listbool" :
  {
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of list of bools",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        # Key                 Type         Default                Req    Check   Help
                        "listbool_opt_def":  ( PythonType.ListBool, [ True, False, True ],  False, None, "Some help string"),
                        "listbool_req":      ( PythonType.ListBool,   None,                 True,  None, "Some help string"),
                        "listbool_opt":      ( PythonType.ListBool,   None,                 False, None, "Some help string"),
                    }
  },

  "TestOptions_liststr" :
  {
    "type"        : "python_module",
    "version"     : "0.1a",
    "description" : "Tests option parsing of list of strings",
    "authors"     : [],
    "refs"        : [],
    "options"     : {
                        # Key                 Type         Default                   Req     Check   Help
                        "liststr_opt_def":  ( PythonType.ListString,    [ "A", "Test", "List" ],  False, None, "Some help string"),
                        "liststr_req":      ( PythonType.ListString,    None,                     True,  None, "Some help string"),
                        "liststr_opt":      ( PythonType.ListString,    None,                     False, None, "Some help string"),
                    }
  },


}


