set(BPMODULE_DATASTORE_FILES
            UIDPointer.cpp
            RegisterUIDPointer.cpp
            OptionHolder.cpp
            OptionBase.cpp
            OptionMap.cpp
            #ModuleGraph.cpp
            export.cpp
   )
  
set(BPMODULE_DATASTORE_HEADERS
            UIDPointer.hpp
            RegisterUIDPointer.hpp
            Wavefunction.hpp
            ModuleGraph.hpp
            ModuleGraphNode.hpp
            CacheData.hpp
            GenericHolder.hpp
            GenericBase.hpp
            OptionMap.hpp
            OptionHolder.hpp
            OptionBase.hpp
            OptionTypes.hpp

            graph/BFS.hpp
            graph/DFS.hpp
            graph/FillMacro.h
            graph/FindSubGraph.hpp
            graph/Graph.hpp
            graph/GraphItr.hpp
   )


ADD_SUPERMODULE(datastore
           "${BPMODULE_DATASTORE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(datastore "${BPMODULE_DATASTORE_HEADERS}" "OptionValidators.py;OptionHolderHelper.py")
