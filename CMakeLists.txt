cmake_minimum_required(VERSION 3.15)

project(sysmonitor-comp LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

add_subdirectory(${CMAKE_SOURCE_DIR}/lib)
add_subdirectory(${CMAKE_SOURCE_DIR}/gui)

# DISABLED FOR NOW
# add_subdirectory(${CMAKE_SOURCE_DIR}/cli)


target_include_directories(sysmonitor PUBLIC ${CMAKE_SOURCE_DIR}/lib/include ${CMAKE_CURRENT_BINARY_DIR}/lib/cmakegen/)
target_link_libraries(sysmonitor PUBLIC ole32 oleaut32 wbemuuid)
target_link_libraries(sysmonitor-gui PUBLIC sysmonitor)
target_include_directories(sysmonitor-gui PRIVATE ${BASE_PROJ_DIR}/include)

# DISABLED FOR NOW
# target_link_libraries(sysmonitor-cli PRIVATE sysmonitor ole32 oleaut32 wbemuuid)
