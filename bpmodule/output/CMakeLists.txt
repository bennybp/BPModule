set(BPMODULE_OUTPUT_FILES
            Output.cpp 
            Output_python.cpp
            export.cpp
   )


ADD_MODULE(output 
           "${BPMODULE_OUTPUT_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(output "")
