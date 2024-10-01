# Define a preprocessor directive for RapidJSON
add_definitions(-DRAPIDJSON_HAS_STDSTRING=1)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/apps 
    ${PROJECT_SOURCE_DIR}/3rdparty
    ${PROJECT_SOURCE_DIR}/3rdparty/rapidjson/include
)

# Set libraries based on the platform
if (UNIX)
    set(LIBRARIES evpp_concurrentqueue event glog pthread)
else()
    set(LIBRARIES evpp_static event glog)
    link_directories(
        ${PROJECT_SOURCE_DIR}/vsprojects/bin/${CMAKE_BUILD_TYPE}/
        ${LIBRARY_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}/
    )
endif()

# Add subdirectories for additional components
add_subdirectory(evnsq)
add_subdirectory(evmc)
