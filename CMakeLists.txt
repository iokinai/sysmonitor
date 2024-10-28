cmake_minimum_required(VERSION 3.15)

project(sysmonitor LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

set(WINDOWS_IMPLEMENTATION 
    src/win/osd_cpu_info.cpp
    src/win/win_wmi_parser.cpp
    src/win/win_wmi.cpp
)

if (WIN32) 
    set(OS_OMPL_HEADERS ${WINDOWS_IMPLEMENTATION})
else()
    message(ERROR "Your OS is not supported")
endif(WIN32)

add_library(sysmonitor SHARED STATIC
    ${OS_OMPL_HEADERS}
)

add_executable(sysmonitor-test
    test/cli_cpu.cpp
)

target_include_directories(sysmonitor PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(sysmonitor PUBLIC ole32 oleaut32 wbemuuid)
target_link_libraries(sysmonitor-test PUBLIC sysmonitor)