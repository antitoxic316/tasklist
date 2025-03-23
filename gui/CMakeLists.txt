add_library(gui mainwindow.c)

include_directories(${CMAKE_SOURCE_DIR}/core)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

target_link_libraries(gui PUBLIC PkgConfig::GTK4)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/path_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/path_config.h @ONLY)

target_include_directories(gui PRIVATE ${CMAKE_CURRENT_BINARY_DIR})


if(DEBUG)
    message(STATUS "Debug build mode")
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_CXX_FLAGS "-g")
    set(CMAKE_C_FLAGS "-g")

    set(CMAKE_EXE_LINKER_FLAGS "-g")
    set(CMAKE_POSITION_INDEPENDENT_CODE OFF)

    set(CMAKE_CXX_FLAGS "-g -O0")
    set(CMAKE_C_FLAGS "-g -O0")
else()
    message(STATUS "Default build mode")
endif()