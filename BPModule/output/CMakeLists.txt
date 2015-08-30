set(BPMODULE_OUTPUT_FILES
            Output.cpp 
            Output_python.cpp
            export.cpp
   )


ADD_MODULE(bpmodule_output 
           "${BPMODULE_OUTPUT_FILES}"
           ""
           ""
           "")

INSTALL_MODULE(bpmodule_output "")
