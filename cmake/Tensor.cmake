
#set(BPMODULE_AMBIT_PATH "" CACHE PATH "Path to Ambit installation")
#if("${BPMODULE_AMBIT_PATH}" STREQUAL "")
#  message(FATAL_ERROR "Ambit path is required!")
#endif() 

