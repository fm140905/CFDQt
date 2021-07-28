add_library(qcustomplot qcustomplot.cpp)
target_link_libraries(qcustomplot PUBLIC Qt5::PrintSupport Qt5::Widgets)

add_library(customplotzoom customplotzoom.cpp)
target_link_libraries(customplotzoom PUBLIC qcustomplot)

add_library(qcustomcanvas qcustomcanvas.cpp plotcanvas.ui)
target_link_libraries(qcustomcanvas customplotzoom)

add_library(mainwindow mainwindow.ui mainwindow.cpp)
target_link_libraries(mainwindow PUBLIC Qt5::Widgets)

add_executable(gui gui.cpp)
target_link_libraries(gui PUBLIC mainwindow)
