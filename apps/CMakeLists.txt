# Define a preprocessor directive for RapidJSON
add_definitions(-DRAPIDJSON_HAS_STDSTRING=1)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/apps 
    ${PROJECT_SOURCE_DIR}/3rdparty
    ${PROJECT_SOURCE_DIR}/3rdparty/rapidjson/include
)

# Set libraries based on the platform

set(LIBRARIES evpp_concurrentqueue event glog pthread)

# Add subdirectories for additional components
add_subdirectory(evnsq)
add_subdirectory(evmc)
