###################################################
# Handling of warnings, etc
###################################################
# Note - -ww2338 has a lot of warnings from python
#        includes. I don't know why these aren't
#        found by -Wswitch-default in GCC. Boost
#        doesn't seem to be supressing any itself
###################################################

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
    list(APPEND PULSAR_CXX_STRICT_FLAGS "-std=c++11")
    list(APPEND PULSAR_CXX_STRICT_FLAGS "-w3")
    list(APPEND PULSAR_CXX_STRICT_FLAGS "-restrict;-DRESTRICT=restrict")

    #  2338  : switch statement does not have a default clause
    #list(APPEND PULSAR_CXX_STRICT_FLAGS "-ww2338")

    # some useless remarks
    #  1418  : external function definition with no prior declaration
    #  1419  : external declaration in primary source file (happens in boost files)
    #   383  : value copied to temporary, reference to temporary used (generally ok in c++)
    #   981  : operands are evaluated in unspecified order (generally ok in c++)
    #   869  : parameter "XXX" was never referenced.
    #          This is due to a bug in Intel before Intel 16 that causes this
    #          remark to be printed for variadic templates
    # 11074  : Inlining inhibited by limit max-size (etc)
    # 11076  : To get full report use -qopt-report=4 -qopt-report-phase ipo
    #   444  : destructor for base class .. not virtual (even warns for classes without virtual functions)
    list(APPEND PULSAR_CXX_STRICT_FLAGS "-wd1418")
    list(APPEND PULSAR_CXX_STRICT_FLAGS "-wd1419")
    list(APPEND PULSAR_CXX_STRICT_FLAGS "-wd383")
    list(APPEND PULSAR_CXX_STRICT_FLAGS "-wd981")
    list(APPEND PULSAR_CXX_STRICT_FLAGS "-wd869")
    list(APPEND PULSAR_CXX_STRICT_FLAGS "-wd11074")
    list(APPEND PULSAR_CXX_STRICT_FLAGS "-wd11076")
    list(APPEND PULSAR_CXX_STRICT_FLAGS "-wd444")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    list(APPEND PULSAR_CXX_STRICT_FLAGS "-std=c++11")
    list(APPEND PULSAR_CXX_STRICT_FLAGS "-DRESTRICT=__restrict__")
    list(APPEND PULSAR_CXX_STRICT_FLAGS "-Wall;-Wextra;-pedantic")
    list(APPEND PULSAR_CXX_STRICT_FLAGS "-Wfloat-equal;-Wshadow")
    list(APPEND PULSAR_CXX_STRICT_FLAGS "-Wswitch-default")
    list(APPEND PULSAR_CXX_STRICT_FLAGS "-Wconversion")

    # maybe in the future. Seems to need a very new gcc compiler
    #list(APPEND PULSAR_CXX_STRICT_FLAGS "-Wmisleading-indentation")
endif()







# Creates a target for a module
macro(ADD_COREMODULE MODULE_NAME
                     MODULE_FILES
                     MODULE_CXX_FLAGS
                     MODULE_CXX_LINK_FLAGS
                     MODULE_CXX_INCLUDES)

  if(NOT "${MODULE_FILES}" STREQUAL "")
      message(STATUS "Adding module ${MODULE_NAME}")
      add_library(${MODULE_NAME} MODULE ${MODULE_FILES} )


      #################
      # CXX Flags
      #################
      list(APPEND ${MODULE_NAME}_CXX_FLAGS ${MODULE_CXX_FLAGS})

      # OpenMP
      list(APPEND ${MODULE_NAME}_CXX_FLAGS ${OpenMP_CXX_FLAGS})

      # MPI
      list(APPEND ${MODULE_NAME}_CXX_FLAGS ${MPI_CXX_COMPILE_FLAGS})

      # Default flags for stuff
      list(APPEND ${MODULE_NAME}_CXX_FLAGS ${PULSAR_CXX_STRICT_FLAGS})
      target_compile_options(${MODULE_NAME} PRIVATE ${${MODULE_NAME}_CXX_FLAGS})



      #################
      # Includes
      #################
      get_filename_component(CMAKE_SOURCE_DIR_PARENT ${CMAKE_SOURCE_DIR} DIRECTORY)
      list(APPEND ${MODULE_NAME}_CXX_INCLUDES ${CMAKE_SOURCE_DIR_PARENT})

      # Passed in
      list(APPEND ${MODULE_NAME}_CXX_INCLUDES ${MODULE_CXX_INCLUDES})

      # Include them
      target_include_directories(${MODULE_NAME} PRIVATE ${${MODULE_NAME}_CXX_INCLUDES})


      # External dependencies (mark as system includes)
      list(APPEND ${MODULE_NAME}_CXX_EXTERNAL_INCLUDES ${Boost_INCLUDE_DIRS})
      list(APPEND ${MODULE_NAME}_CXX_EXTERNAL_INCLUDES ${PULSAR_CORE_PYTHON_INCLUDE_DIRS})
      list(APPEND ${MODULE_NAME}_CXX_EXTERNAL_INCLUDES ${MPI_CXX_INCLUDE_PATH})
      list(APPEND ${MODULE_NAME}_CXX_EXTERNAL_INCLUDES ${PYBIND11_INCLUDE_DIR})
      list(APPEND ${MODULE_NAME}_CXX_EXTERNAL_INCLUDES ${LIBELEMENTAL_INCLUDE_DIR})
      list(APPEND ${MODULE_NAME}_CXX_EXTERNAL_INCLUDES ${MADNESS_INCLUDE_DIRS})
      list(APPEND ${MODULE_NAME}_CXX_EXTERNAL_INCLUDES ${LIBTASKFORCE_INCLUDE_DIRS})
      target_include_directories(${MODULE_NAME} SYSTEM PRIVATE ${${MODULE_NAME}_CXX_EXTERNAL_INCLUDES})


      #################
      # Linker flags
      #################
      # Passed in
      list(APPEND ${MODULE_NAME}_CXX_LINK_FLAGS ${MODULE_CXX_LINK_FLAGS})

      # Main dependencies
      list(APPEND ${MODULE_NAME}_CXX_LINK_FLAGS ${OpenMP_CXX_FLAGS})
      list(APPEND ${MODULE_NAME}_CXX_LINK_FLAGS ${Boost_LIBRARIES})
      list(APPEND ${MODULE_NAME}_CXX_LINK_FLAGS ${PULSAR_CORE_PYTHON_LIBRARIES})
      list(APPEND ${MODULE_NAME}_CXX_LINK_FLAGS ${MPI_CXX_LINK_FLAGS})
      list(APPEND ${MODULE_NAME}_CXX_LINK_FLAGS ${MPI_CXX_LIBRARIES})

      # External interface libraries
      list(APPEND ${MODULE_NAME}_CXX_LINK_FLAGS bphash)



      target_link_libraries(${MODULE_NAME} ${${MODULE_NAME}_CXX_LINK_FLAGS})

  endif()

endmacro()



macro(INSTALL_COREMODULE MODULE_NAME)

  install(TARGETS ${MODULE_NAME} 
          LIBRARY DESTINATION modules/pulsar/${MODULE_NAME}
          RUNTIME DESTINATION modules/pulsar/${MODULE_NAME})

  # Python files are installed from pulsar/CMakeLists.txt

endmacro()
