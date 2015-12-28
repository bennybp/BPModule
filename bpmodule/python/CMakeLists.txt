set(BPMODULE_PYTHON_HELPER_FILES
        Types.cpp
        Errors.cpp
        export.cpp
   )

set(BPMODULE_PYTHON_HELPER_HEADERS
            Pybind11.hpp
            Pybind11_stl.hpp
            Pybind11_functional.hpp
            Pybind11_fwd.hpp
            Types.hpp
            Convert.hpp
            Errors.hpp
            Call.hpp
   )


ADD_SUPERMODULE(python
           "${BPMODULE_PYTHON_HELPER_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(python "${BPMODULE_PYTHON_HELPER_HEADERS}" "")
