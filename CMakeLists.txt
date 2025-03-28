cmake_minimum_required(VERSION 2.8)

project(safe-evpp C CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

if(UNIX)
    set(CXX_FLAGS
        -g -fPIC -Wall -Wextra -D__STDC_FORMAT_MACROS
        -Wno-unused-parameter -Wno-unused-function
        -Wunused-variable -Wunused-value -Wshadow -Wcast-qual
        -Wwrite-strings -Wsign-compare -Winvalid-pch
        -fms-extensions -Wno-implicit-fallthrough -Wfloat-equal
        -std=c++17
    )

    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ggdb -D_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -ggdb -DNDEBUG")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")

    option(ENABLE_GOOGLE_STRIP_LOG "Enable GOOGLE_STRIP_LOG" ON)
    option(LOG_TO_SINK_ONLY "Log to sink only" OFF)

    if(ENABLE_GOOGLE_STRIP_LOG)
        foreach(FLAG CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_RELWITHDEBINFO CMAKE_CXX_FLAGS_MINSIZEREL)
            set(${FLAG} "${${FLAG}} -DGOOGLE_STRIP_LOG=1")
        endforeach()
    else()
        foreach(FLAG CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_RELWITHDEBINFO CMAKE_CXX_FLAGS_MINSIZEREL)
            set(${FLAG} "${${FLAG}} -DGOOGLE_STRIP_LOG=0")
        endforeach()
    endif()

    if(LOG_TO_SINK_ONLY)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGOOGLE_LOG_TO_SINK_ONLY")
    endif()

    include_directories (${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/3rdparty)
    set(DEPENDENT_LIBRARIES event glog pthread)
elseif(WIN32)
    set(CXX_FLAGS -DWIN32 -D_WIN32 -EHsc)
    set(VcpkgRoot ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET})
    include_directories(${VcpkgRoot}/include ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/3rdparty/wininclude)

    # Link directories based on build mode
    link_directories(
        $<$<CONFIG:Debug>:${VcpkgRoot}/debug/lib;${VcpkgRoot}/debug/lib/manual-link>
        $<$<CONFIG:Release>:${VcpkgRoot}/lib;${VcpkgRoot}/lib/manual-link>
    )

    set(DEPENDENT_LIBRARIES event glog)
endif()

if(APPLE)
    set(CMAKE_MACOSX_RPATH 0)
    include_directories(/usr/local/include) # For Intel-based Macs
    link_directories(/usr/local/lib)

    # For Apple Silicon (M1/M2), Homebrew defaults to /opt/homebrew
    if(EXISTS "/opt/homebrew/include")
        include_directories(/opt/homebrew/include)
        link_directories(/opt/homebrew/lib)
    endif()
endif()

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS};${CXX_FLAGS}")

option(ENABLE_HTTPS "Include HTTPS support (openssl required)" OFF)

if(ENABLE_HTTPS)
    list(APPEND DEPENDENT_LIBRARIES ssl crypto event_openssl)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEVPP_HTTP_CLIENT_SUPPORTS_SSL")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEVPP_HTTP_SERVER_SUPPORTS_SSL")
endif(ENABLE_HTTPS)

option(ENABLE_BOOST_FUNCTIONS "Use boost::function instead std::function" OFF)

if(ENABLE_BOOST_FUNCTIONS)
    find_package(Boost REQUIRED)

    if(Boost_FOUND)
        message(STATUS "Boost found: ${Boost_INCLUDE_DIRS}")
    else()
        message(FATAL_ERROR "Boost not found!")
    endif()

    include_directories(${Boost_INCLUDE_DIRS})
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_BOOST_FUNCTION")
endif(ENABLE_BOOST_FUNCTIONS)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

message(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})
message(STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR})
message(STATUS "CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS})
message(STATUS "EXECUTABLE_OUTPUT_PATH " ${EXECUTABLE_OUTPUT_PATH})
message(STATUS "LIBRARY_OUTPUT_PATH " ${LIBRARY_OUTPUT_PATH})

add_subdirectory(evpp)

option(EVPP_VCPKG_BUILD "Disable building examples, tests, applications, or benchmark programs." ON)

if(NOT EVPP_VCPKG_BUILD)
    find_program(THRIFT_COMPILER thrift)
    find_path(THRIFT_INCLUDE_DIR thrift)
    find_library(THRIFT_LIBRARY NAMES thrift)

    message(STATUS "THRIFT_COMPILER " ${THRIFT_COMPILER})
    message(STATUS "THRIFT_INCLUDE_DIR " ${THRIFT_INCLUDE_DIR})
    message(STATUS "THRIFT_LIBRARY " ${THRIFT_LIBRARY})

    add_subdirectory(apps)
    add_subdirectory(labs)
    add_subdirectory(test)
    add_subdirectory(examples)
    add_subdirectory(benchmark)
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(utils)
include(packages)
include(CPack)
include(CTest)
