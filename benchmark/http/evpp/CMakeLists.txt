add_executable(benchmark_http_evpp evpp_http_bench.cc )
target_link_libraries(benchmark_http_evpp evpp_static ${DEPENDENT_LIBRARIES})

