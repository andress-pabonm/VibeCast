cmake_minimum_required(VERSION 3.16)
project(VibeCast)

# Buscar recursivamente todos los archivos .c dentro de src/
file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.c
)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# This assumes the SQLite source is available in vendored/SQLite
add_subdirectory(vendored/SQLite)

# Crear el ejecutable con el mismo nombre que el proyecto
add_executable(${PROJECT_NAME} ${SOURCES})

# Incluir los archivos .h en include/
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link to the actual SDL3 library.
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

# Link to the actual sqlite3 library.
target_link_libraries(${PROJECT_NAME} PRIVATE sqlite3)
