
#set(BPMODULE_CYCLOPS_PATH "" CACHE PATH "Path to Cyclops")
#if("${BPMODULE_CYCLOPS_PATH}" STREQUAL "")
#  message(FATAL_ERROR "Cyclops path is required!")
#endif() 

