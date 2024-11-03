cmake_minimum_required(VERSION 3.30)

project(sysmonitor LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

set(WINDOWS_IMPLEMENTATION 
    src/win/osd_system_info.cpp
    src/win/win_wmi_parser.cpp
    src/win/win_wmi.cpp
)

set(ADDITIONAL_FILES)

set(Boost_ARCHITECTURE "-x64")

add_subdirectory(${CMAKE_SOURCE_DIR}/cli)
add_subdirectory(${CMAKE_SOURCE_DIR}/gui)

set(bstr _bstr_t)

if (WIN32) 
    if (MINGW)
        set(bstr mgw_bstr_t)
        set(ADDITIONAL_FILES src/win/mingw/mgw_bstr_t.cpp)
    endif()

    set(OS_OMPL_HEADERS ${WINDOWS_IMPLEMENTATION})
else()
    message(ERROR "Your OS is not supported")
endif(WIN32)

configure_file(win_types_config.hpp.in cmakegen/win_types_config.hpp)

add_library(sysmonitor STATIC
    ${OS_OMPL_HEADERS}
    ${ADDITIONAL_FILES}
)

add_executable(sysmonitor-cpu-test
    test/cli_cpu.cpp
)

add_executable(sysmonitor-gpu-test
    test/cli_gpu.cpp
)

target_include_directories(sysmonitor PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR}/cmakegen/)
target_link_libraries(sysmonitor PUBLIC ole32 oleaut32 wbemuuid)
target_link_libraries(sysmonitor-cpu-test PUBLIC sysmonitor ole32 oleaut32 wbemuuid)
target_link_libraries(sysmonitor-gpu-test PUBLIC sysmonitor ole32 oleaut32 wbemuuid)
target_link_libraries(sysmonitor-cli PRIVATE sysmonitor ole32 oleaut32 wbemuuid)