set(BPMODULE_UTIL_FILES
            Format.cpp
            Memwatch.cpp
            Cmdline.cpp
            StringUtil.cpp 
            Mangle.cpp
            Filesystem.cpp
            export.cpp

            Hash.cpp
            Hasher.cpp

            vprintfcpp/Format.cpp
            vprintfcpp/Printf_wrap.cpp
   )


ADD_COREMODULE(util 
           "${BPMODULE_UTIL_FILES}"
           ""
           "${MEMWATCH_LIBRARIES}"
           "${MEMWATCH_INCLUDE_DIR}")

set_target_properties(util PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/external/memwatch/lib")

INSTALL_COREMODULE(util)
