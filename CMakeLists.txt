cmake_minimum_required(VERSION 3.31.5)
project(tasklist LANGUAGES C)

add_executable(tasklist main.c)

add_subdirectory(gui)

target_link_libraries(tasklist PUBLIC gui)
target_include_directories(tasklist PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/gui"
                          )

# DEBUG
set(G_MESSAGES_DEBUG all)