#!/usr/bin/env python3

minfo = { "TESTMOD1" :
  {
    "name"        : "Some test",
    "type"        : "c_module",
    "soname"      : "testmodule1.so",
    "version"     : "0.1a",
    "description" : "Some test module",
    "authors"     : ["me", "myself", "I"],
    "refs"        : ["some paper", "some other paper"],
    "options"     : [ 
                        ("adouble", "0.0", "Some help string"),
                        ("aint", 1, "Some help string2"),
                        ("double2", 2.1, "Some help string3")
                    ]
  }
}


