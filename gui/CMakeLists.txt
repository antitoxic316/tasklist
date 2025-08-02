find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)

set(GRESOURCE_C   gui.gresource.c)
set(GRESOURCE_XML gui.gresource.xml)
set(UI_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/addtaskdialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/listboxrow.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/mainwindow.ui
)

add_custom_command(
    OUTPUT ${GRESOURCE_C}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${GLIB_COMPILE_RESOURCES}
    ARGS
        ${GRESOURCE_XML}
        --target=${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
        --generate-source
    VERBATIM
    MAIN_DEPENDENCY ${GRESOURCE_XML}
    DEPENDS
        ${UI_FILES}
)

# Step 3:
add_custom_target(
    gui-gresource
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
)

set_source_files_properties(
    ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
    PROPERTIES GENERATED TRUE
)

add_library(gui SHARED
    ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
    utils.c 
    timeSpinButton.c
    addTaskDialog.c 
    editTaskDialog.c
    tasksboxlist.c 
    mainwindow.c
)

add_dependencies(gui gui-gresource)


include_directories(${CMAKE_SOURCE_DIR}/core)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

target_link_libraries(gui PUBLIC PkgConfig::GTK4)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/path_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/path_config.h @ONLY)

target_include_directories(gui PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

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