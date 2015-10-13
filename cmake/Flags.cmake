###################################################
# Handling of warnings, etc
###################################################
# Note - -ww2338 has a lot of warnings from python
#        includes. I don't know why these aren't
#        found by -Wswitch-default in GCC. Boost
#        doesn't seem to be supressing any itself
###################################################

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
    list(APPEND BPMODULE_CXX_STRICT_FLAGS "-fPIC")
    list(APPEND BPMODULE_CXX_STRICT_FLAGS "-std=c++11")
    list(APPEND BPMODULE_CXX_STRICT_FLAGS "-w3")

    #  2338  : switch statement does not have a default clause
    #list(APPEND BPMODULE_CXX_STRICT_FLAGS "-ww2338")

    # some useless remarks
    #  1418  : external function definition with no prior declaration
    #   411  : class "<unnamed>" defines no constructor to initialize the following (happens in boost::python)
    #  1419  : external declaration in primary source file (happens in boost files)
    #   383  : value copied to temporary, reference to temporary used (generally ok in c++)
    #   981  : operands are evaluated in unspecified order (generally ok in c++)
    list(APPEND BPMODULE_CXX_STRICT_FLAGS "-wd1418")
    list(APPEND BPMODULE_CXX_STRICT_FLAGS "-wd1419")
    list(APPEND BPMODULE_CXX_STRICT_FLAGS "-wd411")
    list(APPEND BPMODULE_CXX_STRICT_FLAGS "-wd383")
    list(APPEND BPMODULE_CXX_STRICT_FLAGS "-wd981")

elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    list(APPEND BPMODULE_CXX_STRICT_FLAGS "-fPIC")
    list(APPEND BPMODULE_CXX_STRICT_FLAGS "-std=c++11")
    list(APPEND BPMODULE_CXX_STRICT_FLAGS "-Wall;-Wextra;-pedantic")
    list(APPEND BPMODULE_CXX_STRICT_FLAGS "-Wfloat-equal;-Wshadow")
    list(APPEND BPMODULE_CXX_STRICT_FLAGS "-Wswitch-default")
    list(APPEND BPMODULE_CXX_STRICT_FLAGS "-Wconversion")

    # maybe in the future. Seems to need a very new gcc compiler
    #list(APPEND BPMODULE_CXX_STRICT_FLAGS "-Wmisleading-indentation")
endif()
