############################
# Find Python libraries
############################
set(Python_ADDITIONAL_VERSIONS "3.3;3.4;3.5;3.6")
find_package(PythonLibs REQUIRED)
string(REGEX REPLACE "^([0-9]+)\\.([0-9]+)\\.[0-9]+.*"
                     "\\1.\\2"
                     PYTHONLIBS_MAJORMINOR_VERSION_STRING
                     "${PYTHONLIBS_VERSION_STRING}")
