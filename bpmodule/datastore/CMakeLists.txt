set(BPMODULE_DATASTORE_FILES
            GenericHolder.cpp
            export.cpp
   )
  
set(BPMODULE_DATASTORE_HEADERS
            Hashable.hpp
            CalcData.hpp
            CacheData.hpp
            PropertyMap.hpp
            GenericHolder.hpp
            GenericBase.hpp
   )


ADD_SUPERMODULE(datastore
           "${BPMODULE_DATASTORE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(datastore "${BPMODULE_DATASTORE_HEADERS}" "")
