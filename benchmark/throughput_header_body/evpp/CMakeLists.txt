include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/3rdparty)

set(LIBRARIES evpp_static ${DEPENDENT_LIBRARIES})

add_executable(benchmark_pingpong_header_body_client client.cc)
target_link_libraries(benchmark_pingpong_header_body_client ${LIBRARIES})

add_executable(benchmark_pingpong_header_body_server server.cc)
target_link_libraries(benchmark_pingpong_header_body_server ${LIBRARIES})
