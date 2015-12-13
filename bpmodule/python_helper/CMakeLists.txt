set(BPMODULE_PYTHON_HELPER_FILES
        Types.cpp
        Errors.cpp
        export.cpp
   )

set(BPMODULE_PYTHON_HELPER_HEADERS
            Pybind11.hpp
            Pybind11_stl.hpp
            BoostPython_fwd.hpp
            Types.hpp
            Convert.hpp
            Errors.hpp
            Call.hpp
   )


ADD_SUPERMODULE(python_helper
           "${BPMODULE_PYTHON_HELPER_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(python_helper "${BPMODULE_PYTHON_HELPER_HEADERS}" "")
