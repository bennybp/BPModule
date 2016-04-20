set(PULSAR_OUTPUT_FILES
            GlobalOutput.cpp
            Output.cpp 
            Table.cpp
            export.cpp
   )


ADD_COREMODULE(output 
           "${PULSAR_OUTPUT_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(output)
