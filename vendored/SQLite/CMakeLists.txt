# Crea una biblioteca estática con el código fuente
add_library(sqlite3 STATIC "sqlite3.c")

# Establece la carpeta de encabezados
target_include_directories(sqlite3 INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

# Opcional: evita advertencias innecesarias de SQLite
# target_compile_definitions(sqlite3 PRIVATE
#     -DSQLITE_THREADSAFE=1
#     -DSQLITE_ENABLE_FTS5
# )