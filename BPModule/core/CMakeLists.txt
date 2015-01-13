##################################################
# Actual library
##################################################
set(CORE_FILES
            ModuleBase.cpp
            ModuleClass.cpp
            ModuleStore.cpp
            )

set(PYTHON_FILES
            Module_python.cpp
           )

set(PYLOADER_FILES
            Python_Loader.cpp
           )

ADD_MODULE(bpmodule_core "${CORE_FILES}" "" "" "") 
ADD_MODULE(bpmodule_python "${PYTHON_FILES}" "" "" "") 
ADD_MODULE(bpmodule_pythonloader "${PYLOADER_FILES}" "" "" "") 


add_subdirectory(base)
