cmake_minimum_required(VERSION 3.15)

project(sysmonitor LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

set(WINDOWS_IMPLEMENTATION 
    src/win/osd_system_info.cpp
    src/win/win_wmi_parser.cpp
    src/win/win_wmi.cpp
)

add_subdirectory(${CMAKE_SOURCE_DIR}/cli)

if (WIN32) 
    set(OS_OMPL_HEADERS ${WINDOWS_IMPLEMENTATION})
else()
    message(ERROR "Your OS is not supported")
endif(WIN32)

add_library(sysmonitor STATIC
    ${OS_OMPL_HEADERS}
)

add_executable(sysmonitor-cpu-test
    test/cli_cpu.cpp
)

add_executable(sysmonitor-gpu-test
    test/cli_gpu.cpp
)

target_include_directories(sysmonitor PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(sysmonitor PUBLIC ole32 oleaut32 wbemuuid)
target_link_libraries(sysmonitor-cpu-test PUBLIC sysmonitor ole32 oleaut32 wbemuuid)
target_link_libraries(sysmonitor-gpu-test PUBLIC sysmonitor ole32 oleaut32 wbemuuid)
target_link_libraries(sysmonitor-cli PRIVATE sysmonitor ole32 oleaut32 wbemuuid)