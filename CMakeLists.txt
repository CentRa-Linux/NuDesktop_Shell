cmake_minimum_required(VERSION 3.14)

project(NuDesktop_Shell VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick Qml Widgets DBus X11Extras Concurrent QuickControls2 LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Qml Widgets DBus X11Extras Concurrent QuickControls2 LinguistTools REQUIRED)
find_package(KF5WindowSystem)
find_package(KF5Kirigami2)
find_package(X11 REQUIRED)

set(PROJECT_SOURCES
        main.cpp appList.h appList.cpp clickItem.h clickItem.cpp clockItem.h clockItem.cpp launcher.h launcher.cpp panel.h panel.cpp window.h window.cpp config.h config.cpp windowSelector.h windowSelector.cpp windowSelectorItem.h windowSelectorItem.cpp windowModel.h windowModel.cpp windowIconProvider.h windowIconProvider.cpp
        qml.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(NuDesktop_Shell
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET NuDesktop_Shell APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(NuDesktop_Shell SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(NuDesktop_Shell
          ${PROJECT_SOURCES}
        )
    endif()
endif()

target_compile_definitions(NuDesktop_Shell
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(NuDesktop_Shell
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::X11Extras Qt${QT_VERSION_MAJOR}::Quick)
target_link_libraries(NuDesktop_Shell PRIVATE KF5::WindowSystem)
target_link_libraries(NuDesktop_Shell PRIVATE KF5::Kirigami2)

set_target_properties(NuDesktop_Shell PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(NuDesktop_Shell)
    qt_finalize_executable(NuDesktop_Shell)
endif()
