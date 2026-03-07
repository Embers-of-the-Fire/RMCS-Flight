cmake_minimum_required(VERSION 3.12)
project(rmcs_core)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-packed-bitfield-compat")

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-O2 -Wall -Wextra -Wpedantic)
endif()

find_package (ament_cmake_auto REQUIRED)
ament_auto_find_build_dependencies ()

file (GLOB_RECURSE PROJECT_SOURCE CONFIGURE_DEPENDS
  ${PROJECT_SOURCE_DIR}/src/*.cpp
  ${PROJECT_SOURCE_DIR}/src/*.c
)

ament_auto_add_library (
  ${PROJECT_NAME} SHARED
  ${PROJECT_SOURCE}
)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/librmcs)

include_directories(SYSTEM "/usr/include/libusb-1.0")

# ---- Odin LiDAR driver (thirdparty, no ROS layer) ----
set(ODIN_DRIVER_DIR "${PROJECT_SOURCE_DIR}/../../thirdparty/odin_ros_driver")

include_directories(SYSTEM "${ODIN_DRIVER_DIR}/include")

execute_process(
    COMMAND uname -m
    OUTPUT_VARIABLE _HOST_ARCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(_HOST_ARCH MATCHES "arm|aarch64")
    set(_ODIN_LIB "${ODIN_DRIVER_DIR}/lib/liblydHostApi_arm.a")
else()
    set(_ODIN_LIB "${ODIN_DRIVER_DIR}/lib/liblydHostApi_amd.a")
endif()

target_link_libraries(${PROJECT_NAME}
    -lusb-1.0
    ${_ODIN_LIB}
    pthread
    ${CMAKE_DL_LIBS}
)

pluginlib_export_plugin_description_file(rmcs_executor plugins.xml)

ament_auto_package()