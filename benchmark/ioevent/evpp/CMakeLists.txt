set (evpp_SRCS evpp_ioevent_bench.cc)
add_executable(benchmark_ioevent_evpp ${evpp_SRCS})
target_link_libraries(benchmark_ioevent_evpp evpp_static ${DEPENDENT_LIBRARIES})

set (evpp_SRCS evpp_ioevent_pipe_watcher.cc)
add_executable(benchmark_ioevent_evpp_pipe_watcher ${evpp_SRCS})
target_link_libraries(benchmark_ioevent_evpp_pipe_watcher evpp_static ${DEPENDENT_LIBRARIES})


