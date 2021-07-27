add_executable(vector3Dtest vector3Dtest.cpp)
target_link_libraries(vector3Dtest PUBLIC Qt5::Gui gtest_main)

add_test(
    NAME vector3Dtest
    COMMAND vector3Dtest
)
    