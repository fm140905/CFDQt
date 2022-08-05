add_executable(neutronSim neutron.cpp)
target_link_libraries(neutronSim PUBLIC cfd)
set_target_properties(neutronSim PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(gammaSim gamma.cpp)
target_link_libraries(gammaSim PUBLIC cfd)
set_target_properties(gammaSim PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
