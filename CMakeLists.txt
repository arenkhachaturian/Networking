cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Set C standard
set(CMAKE_C_STANDARD 11)

# Function to find all source files recursively
function(find_sources dir)
    file(GLOB_RECURSE _sources CONFIGURE_DEPENDS "${dir}/*.c")
    file(GLOB_RECURSE _headers CONFIGURE_DEPENDS "${dir}/*.h")
    set(${dir}_sources ${_sources} PARENT_SCOPE)
    set(${dir}_headers ${_headers} PARENT_SCOPE)
endfunction()

include_directories(src/Client src/Server src/DBReader src/Utils src/Math)

# Find sources for src directory
find_sources("src")

# Add library for src files
add_library(ServerLibrary STATIC ${src_sources})

# Add server executable
add_executable(server TestServer.c)
target_link_libraries(server ServerLibrary)
