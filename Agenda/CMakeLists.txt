
cmake_minimum_required(VERSION 2.7)
set(CMAKE_SYSTEM_NAME Generic) 

# Define installation paths
set(PROJECT_BASE "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/binaries")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
#set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_C_FLAGS "")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSQLITE_THREADSAFE=1")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSQLITE_OMIT_LOAD_EXTENSION")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lpthread ")

#Prepare source files
set(SRC_Files
    "${PROJECT_BASE}/src/app.c"
    "${PROJECT_BASE}/src/agenda/core/db/db.c"
    "${PROJECT_BASE}/src/agenda/core/db/sqlite3.c"
    "${PROJECT_BASE}/src/agenda/core/screen_fsm/add_event_screen.c"
    "${PROJECT_BASE}/src/agenda/core/screen_fsm/default_screen.c"
    "${PROJECT_BASE}/src/agenda/core/screen_fsm/list_events_screen.c"
    "${PROJECT_BASE}/src/agenda/core/screen_fsm/screen_fsm.c"
    "${PROJECT_BASE}/src/agenda/core/screen_fsm/search_screen.c"
    "${PROJECT_BASE}/src/agenda/core/screen_fsm/reminder_screen.c"
    "${PROJECT_BASE}/src/agenda/core/agenda.c"
    "${PROJECT_BASE}/src/agenda/core/agenda_interface.c"
    "${PROJECT_BASE}/src/agenda/core/output_menu.c"
    "${PROJECT_BASE}/src/utils/utils.c"
    )
#Prepare include directories
set(INCLUDE_DIRS 
    "${PROJECT_BASE}/src"
    "${PROJECT_BASE}/src/utils"
    "${PROJECT_BASE}/src/agenda/core/db"
    "${PROJECT_BASE}/src/agenda/core/screen_fsm"
    "${PROJECT_BASE}/src/agenda/core/"
    "${PROJECT_BASE}/src/agenda/config"
    "${PROJECT_BASE}/src/agenda/interface"
    )
# Add include path(s) to the project
include_directories(${INCLUDE_DIRS})
# Add excutable
add_executable(grandma_agenda ${SRC_Files})
