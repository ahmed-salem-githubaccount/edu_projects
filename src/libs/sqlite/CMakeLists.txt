
# ---------- Prepare source files ----------
set(SRC_Files
    "${CMAKE_CURRENT_SOURCE_DIR}/sqlite3.c"
    )

# ---------- Prepare include directories ----------
set(INCLUDE_DIRS 
    "${CMAKE_CURRENT_SOURCE_DIR}"
    )

# ---------- Prepare C compiler flags ----------
set(CMAKE_C_FLAGS "")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSQLITE_OMIT_LOAD_EXTENSION")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lpthread ")

# ---------- Add include path(s) to the project ----------
include_directories(${INCLUDE_DIRS})

# ---------- Add Library ----------
add_library(sqlite3 STATIC ${SRC_Files})

# ---------- Add the lib to Project ----------
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY LINK_LIBRARIES sqlite3)
