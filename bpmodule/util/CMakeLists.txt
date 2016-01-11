set(BPMODULE_UTIL_FILES
            Memwatch.cpp
            Cmdline.cpp
            StringUtil.cpp 
            FormatString.cpp
            Mangle.cpp
            export.cpp
   )


ADD_SUPERMODULE(util 
           "${BPMODULE_UTIL_FILES}"
           ""
           "External::memwatch_interface"
           "")

# TODO - HACK FOR NOW
set_target_properties(util PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/external/memwatch/lib")

INSTALL_COREMODULE(util)
