set(BPMODULE_OPTIONS_FILES
            OptionHolder.cpp
            OptionBase.cpp
            OptionMap.cpp
            export.cpp
   )



ADD_MODULE(options
           "${BPMODULE_OPTIONS_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(options "optionvalidate.py")
