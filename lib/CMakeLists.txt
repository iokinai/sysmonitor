cmake_minimum_required(VERSION 3.15)

project(sysmonitor LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

set(WINDOWS_IMPLEMENTATION 
    src/win/osd_system_info.cpp
    src/win/win_wmi_parser.cpp
    src/win/win_wmi.cpp
)

set(ADDITIONAL_FILES)

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
