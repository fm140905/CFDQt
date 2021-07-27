add_library(geometry geometry.cpp)
target_link_libraries(geometry PUBLIC Qt5::Gui)

add_library(data data.cpp)

add_library(tracking tracking.cpp)
target_link_libraries(tracking PUBLIC geometry data Qt5::Core)

add_library(cfd cfd.cpp)
target_link_libraries(cfd PUBLIC tracking)
