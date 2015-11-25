set(BPMODULE_EXCEPTION_FILES
            GeneralException.cpp
            export.cpp
   )

set(BPMODULE_EXCEPTION_HEADERS
            GeneralException.hpp
            MathException.hpp
            PythonConvertException.hpp
            PythonCallException.hpp
            OptionException.hpp
            ModuleLoadException.hpp
            ModuleCreateException.hpp
            ModuleLocatorException.hpp
            DataStoreException.hpp
            MoleculeException.hpp
            BasisSetException.hpp
   )


ADD_SUPERMODULE(exception
           "${BPMODULE_EXCEPTION_FILES}"
           ""
           ""
           "")


INSTALL_COREMODULE(exception "${BPMODULE_EXCEPTION_HEADERS}" "PyGeneralException.py")
