add_library(geometry geometry.cpp)
target_link_libraries(geometry PUBLIC Qt5::Gui)

add_library(data data.cpp)

add_library(material material.cpp)

add_library(cell cell.cpp)
target_link_libraries(cell PUBLIC geometry data material)

add_library(tracking tracking.cpp)
target_link_libraries(tracking PUBLIC cell)

add_library(cfd cfd.cpp)
target_link_libraries(cfd PUBLIC tracking)
