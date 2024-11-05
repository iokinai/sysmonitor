cmake_minimum_required(VERSION 3.15)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(sysmonitor-gui)

include(cmake/QtCfg.cmake)

set(CXX_UI_FILES
    src/mainwindow.cpp
    src/mainwindow.hpp
    src/sidebarsetup.hpp
    src/pages/cpupage.hpp
    src/pages/cpupage.cpp
    src/pages/pagelayout.hpp
    src/pages/gpupage.hpp
    src/pages/gpupage.cpp
    src/pages/ospage.hpp
    src/pages/ospage.cpp
    src/pages/rampage.hpp
    src/pages/rampage.cpp
)

set(QT_UI_FILES
    src/mainwindow.ui
)

find_package(Qt6 REQUIRED COMPONENTS Widgets Concurrent)

qt_add_executable(sysmonitor-gui
    MANUAL_FINALIZATION
    src/main.cpp

    ${CXX_UI_FILES}
    ${QT_UI_FILES}
    resources.qrc

)


# set_target_properties(sysmonitor-gui PROPERTIES WIN32_EXECUTABLE TRUE)

# set_target_properties(sysmonitor-gui PROPERTIES
#     WIN32_EXECUTABLE TRUE
# )



set(QT_COPY_LIBS
    ${CMAKE_PREFIX_PATH}/bin/Qt6Widgets.dll
    ${CMAKE_PREFIX_PATH}/bin/Qt6Core.dll
    ${CMAKE_PREFIX_PATH}/bin/Qt6Gui.dll
    ${CMAKE_PREFIX_PATH}/bin/Qt6Concurrent.dll
)

set(QT_PLUGINS_DIR ${CMAKE_PREFIX_PATH}/plugins)

add_custom_command(TARGET sysmonitor-gui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${QT_COPY_LIBS}
        "$<TARGET_FILE_DIR:sysmonitor-gui>"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${QT_PLUGINS_DIR} 
        "$<TARGET_FILE_DIR:sysmonitor-gui>/plugins"

)

set_target_properties(sysmonitor-gui PROPERTIES WIN32_EXECUTABLE ON)

target_link_libraries(sysmonitor-gui PRIVATE Qt6::Widgets Qt6::Concurrent)

qt_finalize_executable(sysmonitor-gui)
