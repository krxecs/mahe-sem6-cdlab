find_package(FLEX REQUIRED)

flex_target(cdl05-q1 q1.l ${CMAKE_CURRENT_BINARY_DIR}/q1.c)
add_executable(cdl05-q1 ${FLEX_cdl05-q1_OUTPUTS})
target_link_libraries(cdl05-q1 PRIVATE ${FLEX_LIBRARIES})

flex_target(cdl05-q2 q2.l ${CMAKE_CURRENT_BINARY_DIR}/q2.c)
add_executable(cdl05-q2 ${FLEX_cdl05-q2_OUTPUTS})
target_link_libraries(cdl05-q2 PRIVATE ${FLEX_LIBRARIES})

flex_target(cdl05-q3 q3.l ${CMAKE_CURRENT_BINARY_DIR}/q3.c)
add_executable(cdl05-q3 ${FLEX_cdl05-q3_OUTPUTS})
target_link_libraries(cdl05-q3 PRIVATE ${FLEX_LIBRARIES})

flex_target(cdl05-q4 q4.l ${CMAKE_CURRENT_BINARY_DIR}/q4.c)
add_executable(cdl05-q4 ${FLEX_cdl05-q4_OUTPUTS})
target_link_libraries(cdl05-q4 PRIVATE ${FLEX_LIBRARIES})

flex_target(cdl05-q5 q5.l ${CMAKE_CURRENT_BINARY_DIR}/q5.c)
add_executable(cdl05-q5 ${FLEX_cdl05-q5_OUTPUTS})
target_link_libraries(cdl05-q5 PRIVATE ${FLEX_LIBRARIES})

flex_target(cdl05-q6 q6.l ${CMAKE_CURRENT_BINARY_DIR}/q6.c)
add_executable(cdl05-q6 ${FLEX_cdl05-q6_OUTPUTS})
target_link_libraries(cdl05-q6 PRIVATE ${FLEX_LIBRARIES})
