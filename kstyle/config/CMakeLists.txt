### configuration plugin
add_definitions(-DTRANSLATION_DOMAIN=\"feren_style_config\")
set(kstyle_feren_config_PART_SRCS ferenconfig.cpp)

set(kstyle_feren_config_PART_SRCS ${kstyle_feren_config_PART_SRCS} ferenconfigmodule.cpp)

kconfig_add_kcfg_files(kstyle_feren_config_PART_SRCS ../ferenconfigdata.kcfgc)
set(kstyle_feren_config_PART_FORMS ui/ferenconfig.ui)
ki18n_wrap_ui(kstyle_feren_config_PART_FORMS_HEADERS ${kstyle_feren_config_PART_FORMS})

add_library(kstyle_feren_config MODULE
    ${kstyle_feren_config_PART_SRCS}
    ${kstyle_feren_config_PART_FORMS_HEADERS}
)

target_link_libraries(kstyle_feren_config Qt5::Core Qt5::Gui Qt5::Widgets Qt5::DBus)
target_link_libraries(kstyle_feren_config KF5::ConfigCore KF5::ConfigWidgets KF5::I18n)
install(TARGETS kstyle_feren_config DESTINATION ${QT_PLUGIN_INSTALL_DIR})
install(FILES ferenconfig.desktop DESTINATION  ${SERVICES_INSTALL_DIR})

########### next target ###############
find_package(KF5KCMUtils CONFIG)
set_package_properties(KF5KCMUtils PROPERTIES
    DESCRIPTION "KF5 KCM utils Framework"
    URL "https://projects.kde.org/projects/frameworks/kcmutils"
    TYPE OPTIONAL
    PURPOSE "Required for feren-settings5 application to build")

if( KF5KCMUtils_FOUND )
    set(feren_settings_SOURCES main.cpp)
    add_executable(feren-settings5 ${feren_settings_SOURCES} )
    target_link_libraries(feren-settings5 Qt5::Core Qt5::Gui Qt5::Widgets )
    target_link_libraries(feren-settings5 KF5::I18n KF5::KCMUtils)

    install(TARGETS feren-settings5 ${INSTALL_TARGETS_DEFAULT_ARGS})

    # icon
    ecm_install_icons(ICONS sc-apps-feren-settings.svgz
        DESTINATION ${ICON_INSTALL_DIR}
        THEME hicolor )
endif()
