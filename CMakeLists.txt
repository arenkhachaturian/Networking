cmake_minimum_required(VERSION 3.0)
project(TestServer)

# Set the source files directory
set(SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Add all source files in the src directory to a variable
file(GLOB SOURCES ${SOURCES_DIR}/*.c)

# Add the library target
add_library(ServerLibrary STATIC ${SOURCES})

# Set include directories
target_include_directories(ServerLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Add executable and link it with the library
add_executable(TestServer TestServer.c)
target_link_libraries(TestServer PRIVATE ServerLibrary)
