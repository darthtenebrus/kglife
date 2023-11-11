cmake_minimum_required(VERSION 3.22)
project(kglife)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_FIND_MODULE_DIR})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_DEBUG)
endif()


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        LinguistTools
        REQUIRED)
find_package(KF5 COMPONENTS
        Config
        REQUIRED)
set(LING_OPTIONS -no-obsolete)
qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} mainwindow_ru.ts OPTIONS ${LING_OPTIONS})
qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} mainwindow_de.ts OPTIONS ${LING_OPTIONS})
configure_file(translations.qrc ${CMAKE_BINARY_DIR} COPYONLY)

set(RES_FILES ${CMAKE_BINARY_DIR}/translations.qrc images.qrc)

add_executable(${PROJECT_NAME}
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        KLGameField.cpp
        KLGameField.h
        LoadGameException.h
        DialogItemDelegate.cpp
        DialogItemDelegate.h
        ${RES_FILES}
        ${QM_FILES} configdialog.cpp configdialog.h configdialog.ui)


target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        KF5::ConfigCore
        )


install(TARGETS ${PROJECT_NAME})
