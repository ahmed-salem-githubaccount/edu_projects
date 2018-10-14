
cmake_minimum_required(VERSION 2.7)
set(CMAKE_SYSTEM_NAME Generic) 

# Define installation paths
set(PROJECT_BASE "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#Prepare source files
set(SRC_Files
    "${PROJECT_BASE}/src/main.c"
    "${PROJECT_BASE}/src/agenda/core/db/db.c"
    "${PROJECT_BASE}/src/agenda/core/fsm/fsm_add_event.c"
    "${PROJECT_BASE}/src/agenda/core/fsm/fsm_next_event.c"
    "${PROJECT_BASE}/src/agenda/core/fsm/fsm_list_events.c"
    "${PROJECT_BASE}/src/agenda/core/fsm/fsm.c"
    "${PROJECT_BASE}/src/agenda/core/fsm/fsm_search.c"
    "${PROJECT_BASE}/src/agenda/core/fsm/fsm_reminder.c"
    "${PROJECT_BASE}/src/agenda/core/agenda.c"
    "${PROJECT_BASE}/src/agenda/core/agenda_interface.c"
    "${PROJECT_BASE}/src/agenda/core/console/console.c"
    "${PROJECT_BASE}/src/agenda/utils/utils.c"
    )
#Prepare include directories
set(INCLUDE_DIRS 
    "${PROJECT_BASE}/src"
    "${PROJECT_BASE}/src/agenda/utils"
    "${PROJECT_BASE}/src/agenda/core/db"
    "${PROJECT_BASE}/src/agenda/core/fsm"
    "${PROJECT_BASE}/src/agenda/core/console"
    "${PROJECT_BASE}/src/agenda/core/"
    "${PROJECT_BASE}/src/agenda/config"
    "${PROJECT_BASE}/src/agenda/interface"
    )
    
# Add include path(s) to the project
include_directories(${INCLUDE_DIRS})

# Define project name
project("grandma_agenda")

set(CMAKE_C_FLAGS "")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSQLITE_OMIT_LOAD_EXTENSION")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lpthread ")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-incompatible-pointer-types")

# Add excutable
add_executable(grandma_agenda ${SRC_Files})

# Add sub directories
add_subdirectory(src/libs)
