#########################################################################################################
###                                       GENERAL CONFIG                                               ##
#########################################################################################################

message("%%% SYNTHESIS PROJECT : DEMO ARKINS")
message("\n%% GENERAL CONFIGURATION")

# GENERAL SETTINGS
project(DemoDrone CXX)
cmake_minimum_required(VERSION 3.2)
set(VERSION 1.0.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(APP_NAME DemoDrone)

# OUTPUT DIR SETTINGS
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)

# COMPILER OPTIONS
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG   " -g3 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE " -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") # Warning : option are important for some lib


########################################################################################################
###                                       LIBRARY CONFIG                                              ##
########################################################################################################

message("\n%% EXTERNALS libs configuration")

message("- Checking : SFML")
find_library(SFML_LIB sfml REQUIRED)

message("- Checking : Blc channels")
find_package(blc_channel REQUIRED)
find_package(blc_program REQUIRED)
include_directories( ${BL_INCLUDE_DIRS} )
add_definitions( ${BL_DEFINITIONS} )

message("- Checking : libconfig")
find_library(libconfig++ libconfig++ REQUIRED)

message("- Checking : OpenCV")
find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})

message("- Checking : Thread")
find_package(Threads)


######################################################################################################
###                                            LINKS                                                ##
######################################################################################################

link_libraries(
    ${CMAKE_THREAD_LIBS_INIT}
    ${BL_LIBRARIES}
    dl
    config++
)

add_subdirectory(lib)
include_directories(lib)
include_directories(src)

######################################################################################################
###                                         APPLICATION                                             ##
######################################################################################################

message("\n%% APPLICATION GENERATION")

file(GLOB_RECURSE project_SRC 
    "lib/*.h"
    "src/*.h"
    "src/*.cpp"
)

# Copy the config file to the bin folder to make it run
file(COPY config/config.cfg DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# %% App files
add_executable(${APP_NAME} ${project_SRC})

target_link_libraries(${APP_NAME} PUBLIC
    ${OpenCV_LIBS}
    loguru
    nlohmann
    -lsfml-graphics
    -lsfml-window
    -lsfml-system
    -lconfig++
)