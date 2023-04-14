cmake_minimum_required(VERSION 3.5)

project(MedicalDetector VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


list(APPEND CMAKE_PREFIX_PATH ./3rdparty/cuttedOpencv/)


add_definitions(-DENABLE_MEDICAL_DETECTOR=OFF)


find_package(glog REQUIRED)
find_package(OpenCV REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets  WebEngineWidgets Qml)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets WebEngineWidgets Qml)
enable_testing()


if(CMAKE_BUILD_TYPE MATCHES "Debug")
    add_definitions(-DDEBU)
endif()

add_subdirectory(test)
add_subdirectory(DataManager)



set(VIPSPATH 3rdparty/vips-dev-8.14)

list(APPEND ${CMAKE_PREFIX_PATH} ${VIPSPATH})

include_directories(${VIPSPATH}/include)
include_directories(${VIPSPATH}/include/glib-2.0)
include_directories(${VIPSPATH}/lib/glib-2.0/include)
link_directories(${VIPSPATH}/lib)
link_directories(./3rdparty/cuttedOpencv/lib)

if(WIN32)
set(icon_RC medicalDetecter.rc)
set_property(SOURCE medicalDetecter.rc APPEND PROPERTY
        OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/icon/logo.ico
        )
endif()
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.hpp
        mainwindow.ui
        vipswrapper.hpp vipswrapper.cpp
        resources.qrc
        networkmanager.hpp networkmanager.cpp
        inputcheckinfowidget.hpp inputcheckinfowidget.cpp inputcheckinfowidget.ui
        modifycheckinfodialog.hpp modifycheckinfodialog.cpp modifycheckinfodialog.ui
        webviewwidget.hpp webviewwidget.cpp
        qmlcommunicator.hpp qmlcommunicator.cpp
        logwidget.cpp
        logwidget.hpp
        logwidget.ui
        myspliter.hpp myspliter.cpp

        defs.h

        CoreFunctionality/cutpyramid.hpp CoreFunctionality/cutpyramid.cpp
        CoreFunctionality/checkbackgroundandmove.hpp CoreFunctionality/checkbackgroundandmove.cpp
        CoreFunctionality/medicaldetector.hpp CoreFunctionality/medicaldetector.cpp




)

#set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/resources/images/logo.ico")
make_directory(${CMAKE_CURRENT_BINARY_DIR}/preHtml)
make_directory(${CMAKE_CURRENT_BINARY_DIR}/postHtml)

file(GLOB RUNTIME_DLLS "${CMAKE_CURRENT_SOURCE_DIR}/runtime/*" )
file(COPY ${RUNTIME_DLLS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB HTMLS "${CMAKE_CURRENT_SOURCE_DIR}/resources/html/preHtml/*" )
file(COPY ${HTMLS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/preHtml)




file(GLOB HTMLS1 "${CMAKE_CURRENT_SOURCE_DIR}/resources/html/postHtml/*" )
file(COPY ${HTMLS1} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/postHtml)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    if(WIN32)
    qt_add_executable(MedicalDetector WIN32 ${icon_RC}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    else()
        qt_add_executable(MedicalDetector
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
        )
    endif()
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MedicalDetector APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(MedicalDetector SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MedicalDetector
            ${PROJECT_SOURCES}
        )
    endif()
endif()

foreach(lib ${OpenCV_LIBS})
    message("opencv:${lib}")
endforeach()

target_link_libraries(MedicalDetector PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Qml glog::glog
    ${GLIB2_LIBRARIES} libvips.lib libgobject-2.0.lib Qt6::WebEngineWidgets LDataManager ${OpenCV_LIBS}
   )


set_target_properties(MedicalDetector PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS MedicalDetector
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MedicalDetector)
endif()
