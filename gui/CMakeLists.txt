add_library(gui mainwindow.c tasklistbox.c)

include_directories(${CMAKE_SOURCE_DIR}/core)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

target_link_libraries(gui PUBLIC PkgConfig::GTK4)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

target_include_directories(gui PRIVATE ${CMAKE_CURRENT_BINARY_DIR})