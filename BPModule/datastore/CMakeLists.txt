set(BPMODULE_DATASTORE_FILES
            PropertyMap.cpp
            PropertyMap_python.cpp
            export.cpp
   )
  


ADD_MODULE(bpmodule_datastore
           "${BPMODULE_DATASTORE_FILES}"
           ""
           ""
           "")

INSTALL_MODULE(bpmodule_datastore "")
