set(BPMODULE_OPTIONS_FILES
            OptionHolder.cpp
            OptionBase.cpp
            OptionMap.cpp
            export.cpp
   )

set(BPMODULE_OPTIONS_HEADERS
            OptionTypes.hpp
            OptionMap.hpp
            OptionBase.hpp
            OptionHolder.hpp
   )



ADD_MODULE(options
           "${BPMODULE_OPTIONS_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(options "${BPMODULE_OPTIONS_HEADERS}" "optionvalidate.py")
