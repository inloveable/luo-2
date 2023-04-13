cmake_minimum_required(VERSION 3.5)

project(TestDzSave LANGUAGES CXX)




set(VIPSPATH ../vips-dev-8.14)

include_directories(${VIPSPATH}/include)
include_directories(${VIPSPATH}/include/glib-2.0)
include_directories(${VIPSPATH}/lib/glib-2.0/include)
link_directories(${VIPSPATH}/lib)



#
list(APPEND CMAKE_PREFIX_PATH "C:/Program Files (x86)/googletest-distribution" )
list(APPEND CMAKE_PREFIX_PATH ../3rdparty/cuttedOpencv/)


find_package(OpenCV REQUIRED)
find_package(glog REQUIRED)
find_package(GTest REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)


set(GTestFiles
    ../vipswrapper.hpp
    ../vipswrapper.cpp
    ../CoreFunctionality/cutpyramid.hpp
    ../CoreFunctionality/cutpyramid.cpp
    ../CoreFunctionality/checkbackgroundandmove.hpp
    ../CoreFunctionality/checkbackgroundandmove.cpp

    maintest.hpp maintest.cpp
    testnetworkmanager.hpp testnetworkmanager.cpp
)


add_executable(TestDzSave main.cpp
               ${GTestFiles})

target_link_libraries(TestDzSave PRIVATE Qt${QT_VERSION_MAJOR}::Widgets glog::glog ${OpenCV_LIBS}

    Qt${QT_VERSION_MAJOR}::Sql
    ${GLIB2_LIBRARIES} ${GTEST_BOTH_LIBRARIES}
    libvips.lib
    libglib-2.0.lib
    libgobject-2.0.lib libgsf-1.lib)

