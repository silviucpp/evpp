file(GLOB SROUCES *.cc *.h)
add_executable(test_evpp_stability ${SROUCES})
target_link_libraries(test_evpp_stability evpp_static ${DEPENDENT_LIBRARIES})

if (UNIX)
    add_executable(test_evpp_stability_boost_lockfree ${SROUCES})
    target_link_libraries(test_evpp_stability_boost_lockfree evpp_boost_static ${DEPENDENT_LIBRARIES})

    add_executable(test_evpp_stability_concurrentqueue ${SROUCES})
    target_link_libraries(test_evpp_stability_concurrentqueue evpp_concurrentqueue_static ${DEPENDENT_LIBRARIES})
endif (UNIX)

