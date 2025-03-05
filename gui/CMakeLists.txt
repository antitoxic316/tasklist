add_library(gui mainwindow.c tasklistbox.c)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

target_link_libraries(gui PUBLIC PkgConfig::GTK4 core)