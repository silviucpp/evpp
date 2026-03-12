
include_directories(${PROJECT_SOURCE_DIR}/apps
                    ${PROJECT_SOURCE_DIR}/labs
                    ${PROJECT_SOURCE_DIR}/3rdparty)

set(LIBRARIES evpp_concurrentqueue event glog pthread)
link_directories(${PROJECT_BUILD_DIR}/lib)

if(THRIFT_COMPILER AND THRIFT_INCLUDE_DIR AND THRIFT_LIBRARY)
    add_subdirectory(evthrift)
endif()

