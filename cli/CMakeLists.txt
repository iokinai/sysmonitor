cmake_minimum_required(VERSION 3.15)

project(sysmonitor-cli)

set(CMAKE_CXX_STANDARD 23)

find_package(Boost REQUIRED COMPONENTS program_options)

add_executable(sysmonitor-cli
    src/main.cpp
    src/formatter.cpp
)

target_include_directories(sysmonitor-cli PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(sysmonitor-cli PRIVATE ${Boost_LIBRARIES})