cmake_minimum_required (VERSION 2.8)

project (safe-evpp C CXX)

if (UNIX)
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release")
    endif()

    set (CXX_FLAGS
         -g
         -fPIC
         -Wall
         -Wextra
         -D__STDC_FORMAT_MACROS
         -Wno-unused-parameter
         -Wno-unused-function
         -Wunused-variable
         -Wunused-value
         -Wshadow
         -Wcast-qual
         -Wwrite-strings
         -Wsign-compare
         -Winvalid-pch
         -fms-extensions
         -Wno-implicit-fallthrough
         -Wfloat-equal
         -std=c++17
    )
    include_directories (
        ${PROJECT_SOURCE_DIR} 
        ${PROJECT_SOURCE_DIR}/3rdparty
    )
else (UNIX)
    set (CXX_FLAGS -DWIN32 -D_WIN32 -EHsc)
    set(VcpkgRoot ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET})
    include_directories(
        ${VcpkgRoot}/include
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/3rdparty/wininclude
    )

    # On windows, Debug mode
    link_directories(${VcpkgRoot}/debug/lib ${VcpkgRoot}/debug/lib/manual-link)

    # On Windows, Release mode
    # link_directories(${VcpkgRoot}/lib ${VcpkgRoot}/lib/manual-link)
endif (UNIX)

if(APPLE)
    include_directories(/usr/local/include) # For Intel-based Macs
    link_directories(/usr/local/lib)

    # For Apple Silicon (M1/M2), Homebrew defaults to /opt/homebrew
    if(EXISTS "/opt/homebrew/include")
        include_directories(/opt/homebrew/include)
        link_directories(/opt/homebrew/lib)
    endif()
endif()

string (REPLACE ";" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS};${CXX_FLAGS}")

if (UNIX)
    SET(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g -ggdb -D_DEBUG -DGOOGLE_STRIP_LOG=0")
    SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -g -ggdb -DNDEBUG -DGOOGLE_STRIP_LOG=1")
    SET(DEPENDENT_LIBRARIES event glog pthread)
else (UNIX)
    SET(DEPENDENT_LIBRARIES event glog)
endif (UNIX)

# Set to true if https support is needed.
# Note that this needs openssl
# SET(HTTPS True)
if (HTTPS)
    list(APPEND DEPENDENT_LIBRARIES ssl crypto event_openssl)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEVPP_HTTP_CLIENT_SUPPORTS_SSL")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEVPP_HTTP_SERVER_SUPPORTS_SSL")
endif (HTTPS)

if (CMAKE_BENCHMARK_TESTING)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DH_BENCHMARK_TESTING=1")
endif (CMAKE_BENCHMARK_TESTING)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

#set (MACOSX_RPATH ${LIBRARY_OUTPUT_PATH})
set (CMAKE_MACOSX_RPATH 0)

message (STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})
message (STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR})
message (STATUS "CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS})
message (STATUS "EXECUTABLE_OUTPUT_PATH " ${EXECUTABLE_OUTPUT_PATH})
message (STATUS "LIBRARY_OUTPUT_PATH " ${LIBRARY_OUTPUT_PATH})

add_subdirectory (evpp)

if (NOT EVPP_VCPKG_BUILD)
    find_program(THRIFT_COMPILER thrift)
    find_path(THRIFT_INCLUDE_DIR thrift)
    find_library(THRIFT_LIBRARY NAMES thrift)

    message (STATUS "THRIFT_COMPILER " ${THRIFT_COMPILER})
    message (STATUS "THRIFT_INCLUDE_DIR " ${THRIFT_INCLUDE_DIR})
    message (STATUS "THRIFT_LIBRARY " ${THRIFT_LIBRARY})

    add_subdirectory (apps)
    add_subdirectory (labs)
    add_subdirectory (test)
    add_subdirectory (examples)
    add_subdirectory (benchmark)
endif ()

set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include (utils)
include (packages)
include (CPack)
include (CTest)
