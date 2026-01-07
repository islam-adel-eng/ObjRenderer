CMAKE_MINIMUM_REQUIRED (VERSION 4.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(ObjRenderer)

# Define include directories
include_directories("lib/SDL3/include") # SDL3's include
include_directories("include") # My own include

add_subdirectory(lib/SDL3 EXCLUDE_FROM_ALL) # add SDL3 as a subdirectory (it has it's own CMakeLists.txt)

set(SOURCES
    src/main.c
    src/ObjParser.c
) # Add source files present in src

add_executable(${PROJECT_NAME} ${SOURCES}) # Compile source files into an excutable named after the project

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3) # link SDL3 against the excutable
