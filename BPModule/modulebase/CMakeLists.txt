##################################################
# Actual library
##################################################

set(MODULEBASE_FILES
            Test_Base.cpp
   )


set(MODULEBASE_HEADERS
            Test_Base.hpp
   )

ADD_CORELIB(bpmodule_modulebase "${MODULEBASE_FILES}" "" "" "")
install(FILES ${MODULEBASE_HEADERS} DESTINATION include/modulebase)

