set(BPMODULE_DATASTORE_FILES
            GenericHolder.cpp
            export.cpp
   )
  


ADD_MODULE(datastore
           "${BPMODULE_DATASTORE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(datastore "")
