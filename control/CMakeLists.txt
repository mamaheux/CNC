cmake_minimum_required(VERSION 3.1.0)

project(control VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 REQUIRED COMPONENTS Widgets SerialPort Charts OpenGL)

include_directories(include)
include_directories(../firmware/teensy/lib/cnc/include)
include_directories(optional/include)

file(GLOB_RECURSE
    source_files
    src/*
    include/*
    ../firmware/teensy/lib/cnc/include/*
    ../firmware/teensy/lib/cnc/src/*
)

add_executable(control
    ${source_files}
    resources/resources.qrc
)

target_link_libraries(control
    Qt5::Widgets
    Qt5::SerialPort
    Qt5::Charts
    Qt5::OpenGL
    GL
    GLU
)

