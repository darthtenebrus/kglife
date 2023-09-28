cmake_minimum_required(VERSION 3.23)
project(kglife)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_DEBUG)
endif()


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        LinguistTools
        REQUIRED)
set(LING_OPTIONS -no-obsolete)
qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} mainwindow_ru.ts OPTIONS ${LING_OPTIONS})
configure_file(translations.qrc ${CMAKE_BINARY_DIR} COPYONLY)

set(RES_FILES ${CMAKE_BINARY_DIR}/translations.qrc images.qrc)

add_executable(${PROJECT_NAME}
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        KLGameField.cpp
        KLGameField.h
        ${RES_FILES}
        ${QM_FILES})

target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

