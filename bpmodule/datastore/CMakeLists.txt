set(BPMODULE_DATASTORE_FILES
            PropHolder.cpp
            PropertyMap_python.cpp
            export.cpp
   )
  


ADD_MODULE(datastore
           "${BPMODULE_DATASTORE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(datastore "")
