add_executable(vector3Dtest vector3Dtest.cpp)
target_link_libraries(vector3Dtest PUBLIC Qt5::Gui gtest_main)
# set_target_properties(vector3Dtest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
add_test(
    NAME vector3Dtest
    COMMAND vector3Dtest
)

add_executable(geometryTest geometryTest.cpp)
target_link_libraries(geometryTest PUBLIC geometry gtest_main)
# set_target_properties(geometryTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
add_test(
    NAME geometryTest
    COMMAND geometryTest
)
    
add_executable(dataTest dataTest.cpp)
target_link_libraries(dataTest PUBLIC data gtest_main)
# set_target_properties(dataTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
add_test(
    NAME dataTest
    COMMAND dataTest
)

add_executable(cellTest cellTest.cpp)
target_link_libraries(cellTest PUBLIC cell gtest_main)
add_test(
    NAME cellTest
    COMMAND cellTest
)
    