add_library(core task.c tasklist_serializer.c tasklist.c)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/core_path_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/core_path_config.h @ONLY)

target_include_directories(core PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

if(DEBUG)
    message(STATUS "Debug build mode")
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0")

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g")
    set(CMAKE_POSITION_INDEPENDENT_CODE OFF)
else()
    message(STATUS "Default build mode")
endif()