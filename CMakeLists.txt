cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(SOURCE_TEMPLATES templates)
set(DOC_DIR doc)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(ECMSetupVersion)
set(KGLIFE_VERSION "1.0.9")
ecm_setup_version(${KGLIFE_VERSION}
        VARIABLE_PREFIX KGLIFE
        VERSION_HEADER kglife_version.h)
project(kglife VERSION ${KGLIFE_VERSION})

include(ECMInstallIcons)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_DEBUG)
endif()


find_package(Qt5 COMPONENTS
        PrintSupport
        REQUIRED)
find_package(KF5 COMPONENTS
        Config
        Crash
        ConfigWidgets
        I18n
        DocTools
        XmlGui
        REQUIRED)


set(RES_FILES images.qrc ${PROJECT_NAME}.qrc)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
        main.cpp
        mainwindow.cpp
        mainwindow.h
        KLGameField.cpp
        KLGameField.h
        LoadGameException.h
        myslider.cpp
        myslider.h
        generalpage.cpp
        generalpage.h
        generalpage.ui
        patternspage.cpp
        patternspage.h
        patternspage.ui
        cellsgenerator.cpp
        cellsgenerator.h
        generatorpage.cpp
        generatorpage.h
        generatorpage.ui
        ${RES_FILES})

set_property(TARGET ${PROJECT_NAME} PROPERTY AUTOUIC_OPTIONS --tr i18n --idbased --include KLocalizedString)
kde_target_enable_exceptions(${PROJECT_NAME} PRIVATE)

kconfig_add_kcfg_files(${PROJECT_NAME} ${PROJECT_NAME}.kcfgc)
kconfig_add_kcfg_files(${PROJECT_NAME}_SRCS GENERATE_MOC ${PROJECT_NAME}.kcfgc)


target_link_libraries(${PROJECT_NAME}
        Qt5::PrintSupport
        KF5::ConfigCore
        KF5::ConfigWidgets
        KF5::I18n
        KF5::Crash
        KF5::XmlGui
        )


install(TARGETS ${PROJECT_NAME} ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES org.kde.${PROJECT_NAME}.desktop  DESTINATION  ${KDE_INSTALL_APPDIR})

ecm_install_icons(ICONS
        desktop/64-apps-${PROJECT_NAME}.png
        desktop/48-apps-${PROJECT_NAME}.png
        desktop/32-apps-${PROJECT_NAME}.png
        desktop/24-apps-${PROJECT_NAME}.png
        DESTINATION ${KDE_INSTALL_ICONDIR}
        THEME hicolor)


install(FILES ${PROJECT_NAME}.kcfg  DESTINATION  ${KDE_INSTALL_KCFGDIR})
install(FILES ${PROJECT_NAME}ui.rc  DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/${PROJECT_NAME})
install(FILES org.kde.${PROJECT_NAME}.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_TEMPLATES} DESTINATION ${KDE_INSTALL_FULL_DATAROOTDIR}/${PROJECT_NAME})

kdoctools_create_handbook(${DOC_DIR}/index.docbook INSTALL_DESTINATION ${KDE_INSTALL_DOCBUNDLEDIR}/en SUBDIR ${PROJECT_NAME})
kdoctools_create_manpage(${DOC_DIR}/man-kglife.1.docbook 1 INSTALL_DESTINATION ${MAN_INSTALL_DIR})

kdoctools_install(po)
ki18n_install(po)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)


