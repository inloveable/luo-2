cmake_minimum_required(VERSION 3.5)

project(LDataManager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

find_package(glog REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets sql PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets  sql PrintSupport)


set(PROJECT_SOURCES
    datamanager.h datamanager.cpp

    libqsql.cpp
    libqsql.h
    mprinter.cpp
    mprinter.h
    progresscontroller.hpp progresscontroller.cpp
)

add_library(LDataManager OBJECT ${PROJECT_SOURCES})


target_link_libraries(LDataManager PRIVATE
      Qt${QT_VERSION_MAJOR}::Sql glog::glog Qt${QT_VERSION_MAJOR}::PrintSupport)
target_include_directories(LDataManager PUBLIC .)

set_target_properties(LDataManager PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
