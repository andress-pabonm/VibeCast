cmake_minimum_required(VERSION 3.28)
project(VibeCast VERSION 1.0 LANGUAGES C CXX)

# Verificar que estamos en Windows
if(NOT WIN32)
    message(FATAL_ERROR "Este proyecto solo puede compilarse en Windows con MSVC.")
endif()

# Verificar que el compilador es MSVC
if(NOT CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    message(FATAL_ERROR "Este proyecto requiere el compilador MSVC. Detectado: ${CMAKE_C_COMPILER_ID}")
endif()

# (Opcional) Puedes verificar la ruta del compilador también:
get_filename_component(COMPILER_PATH ${CMAKE_C_COMPILER} DIRECTORY)
message(STATUS "Compilador detectado: ${CMAKE_C_COMPILER_ID} en ${COMPILER_PATH}")


# Agregar una opción para compilar con o sin consola
option(BUILD_GUI_APP "Compilar como aplicación gráfica (sin consola)" OFF)

# Según la opción definir la siguiente variable
if(BUILD_GUI_APP)
    set(CMAKE_WIN32_EXECUTABLE TRUE)
else()
    set(CMAKE_WIN32_EXECUTABLE FALSE)
endif()

# Buscar el código fuente del proyecto (.c)
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

# Definir el nombre del ejecutable
set(EXECUTABLE_NAME ${PROJECT_NAME})

# Configurar el archivo de constantes para el proyecto
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.h.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}Config.h"
)

# Incluir las librerías

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

# Hacer disponibles lasa librerías
FetchContent_MakeAvailable(webview pcre2 json-c)

# Añadir el directorio del SQLite
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendored/sqlite")

# Crear el ejecutable
add_executable(${EXECUTABLE_NAME} ${SOURCES})

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
