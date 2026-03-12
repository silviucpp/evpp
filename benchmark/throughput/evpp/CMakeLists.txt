include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/3rdparty)

set(LIBRARIES evpp_static ${DEPENDENT_LIBRARIES})

add_executable(benchmark_pingpong_client client.cc)
target_link_libraries(benchmark_pingpong_client ${LIBRARIES})

add_executable(benchmark_pingpong_client_fixed_size client_fixed_size.cc)
target_link_libraries(benchmark_pingpong_client_fixed_size ${LIBRARIES})

add_executable(benchmark_pingpong_server server.cc)
target_link_libraries(benchmark_pingpong_server ${LIBRARIES})
