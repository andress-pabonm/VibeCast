cmake_minimum_required(VERSION 4.0)
project(VibeCast VERSION 1.0 LANGUAGES C CXX)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

set(EXECUTABLE_NAME ${PROJECT_NAME})

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.h.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}Config.h"
)

include(FetchContent)

# ──────────────────────────────────────────────
# Webview - Para la interfaz gráfica
FetchContent_Declare(
    webview
    GIT_REPOSITORY https://github.com/webview/webview.git
    GIT_TAG 0.12.0
)

# ──────────────────────────────────────────────
# PCRE2 - Para expresiones regulares
FetchContent_Declare(
    pcre2
    GIT_REPOSITORY https://github.com/PCRE2Project/pcre2.git
    GIT_TAG pcre2-10.45
)

# ──────────────────────────────────────────────
# JSON-C - Para manejar el formato JSON
FetchContent_Declare(
    json-c
    GIT_REPOSITORY https://github.com/json-c/json-c.git
    GIT_TAG json-c-0.18-20240915
)

# Opciones para evitar construir ejecutables innecesarios
set(PCRE2_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(PCRE2_BUILD_PCRE2GREP OFF CACHE BOOL "" FORCE)
set(PCRE2_BUILD_PCRE2TEST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(webview pcre2 json-c)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendored/sqlite")

option(DISABLE_CLI "Para deshabilitar la linea de comandos" OFF)
if(DISABLE_CLI)
    add_executable(${EXECUTABLE_NAME} WIN32)
else()
    add_executable(${EXECUTABLE_NAME})
endif()

target_sources(${EXECUTABLE_NAME} PRIVATE ${SOURCES})

target_link_libraries(
    ${EXECUTABLE_NAME}
    PRIVATE
    sqlite3
    webview::core_static
    pcre2-8-static
    json-c
)

target_link_libraries(${EXECUTABLE_NAME}
    PRIVATE
    advapi32
    ole32
    shell32
    shlwapi
    user32
    version
)

target_include_directories(
    ${EXECUTABLE_NAME}
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# install(
#     TARGETS ${EXECUTABLE_NAME}
#     DESTINATION bin
# )
# install(
#     FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"
#     DESTINATION include
# )

include(InstallRequiredSystemLibraries)
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_GENERATOR "ZIP")
set(CPACK_SOURCE_GENERATOR "ZIP")
include(CPack)
