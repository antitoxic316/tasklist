cmake_minimum_required(VERSION 3.31.5)
project(tasklist LANGUAGES C)

add_executable(tasklist main.c)

add_subdirectory(gui)
add_subdirectory(core)

target_link_libraries(tasklist PUBLIC gui)
target_include_directories(tasklist PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/gui"
                          "${PROJECT_SOURCE_DIR}/core"
                          )

target_link_libraries(gui PUBLIC core)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lm -ldl")

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