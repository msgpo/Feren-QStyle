################# Qt/KDE #################
find_package(Qt5 REQUIRED CONFIG COMPONENTS Widgets DBus)
find_package(KF5 REQUIRED COMPONENTS
    I18n
    Config
    GuiAddons
    IconThemes
    ConfigWidgets
    WindowSystem)

find_package(Qt5 COMPONENTS Quick)
set(FEREN_HAVE_QTQUICK ${Qt5Quick_FOUND})

find_package( KF5FrameworkIntegration CONFIG )
set_package_properties(KF5FrameworkIntegration PROPERTIES
    DESCRIPTION "KF5 Framework Integration"
    URL "https://projects.kde.org/projects/frameworks/frameworkintegration"
    TYPE OPTIONAL
    PURPOSE "Required to use KStyle convenience functionalities in style")

set(FEREN_HAVE_KSTYLE ${KF5FrameworkIntegration_FOUND})

find_package(XCB COMPONENTS XCB)
set_package_properties(XCB PROPERTIES
    DESCRIPTION "X protocol C-language Binding"
    URL "https://xcb.freedesktop.org"
    TYPE OPTIONAL
    PURPOSE "Required to pass style properties to native Windows on X11 Platform"
)

find_package(KF5Wayland CONFIG)
set(FEREN_HAVE_KWAYLAND ${KF5Wayland_FOUND})

if(UNIX AND NOT APPLE)
    set(FEREN_HAVE_X11 ${XCB_XCB_FOUND})
    if (XCB_XCB_FOUND)
        find_package(Qt5 REQUIRED CONFIG COMPONENTS X11Extras)
    endif()
else()
    set(FEREN_HAVE_X11 FALSE)
endif()


################# includes #################
include_directories(
  animations
  debug
)

include_directories(${CMAKE_SOURCE_DIR}/libferencommon)
include_directories(${CMAKE_BINARY_DIR}/libferencommon)

################# configuration #################
configure_file(config-feren.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-feren.h )

########### next target ###############
set(feren_PART_SRCS
    animations/ferenanimation.cpp
    animations/ferenanimations.cpp
    animations/ferenanimationdata.cpp
    animations/ferenbaseengine.cpp
    animations/ferenbusyindicatordata.cpp
    animations/ferenbusyindicatorengine.cpp
    animations/ferendialdata.cpp
    animations/ferendialengine.cpp
    animations/ferenenabledata.cpp
    animations/ferengenericdata.cpp
    animations/ferenheaderviewdata.cpp
    animations/ferenheaderviewengine.cpp
    animations/ferenscrollbardata.cpp
    animations/ferenscrollbarengine.cpp
    animations/ferenspinboxengine.cpp
    animations/ferenspinboxdata.cpp
    animations/ferenstackedwidgetdata.cpp
    animations/ferenstackedwidgetengine.cpp
    animations/ferentabbarengine.cpp
    animations/ferentabbardata.cpp
    animations/ferentoolboxengine.cpp
    animations/ferentransitiondata.cpp
    animations/ferentransitionwidget.cpp
    animations/ferenwidgetstateengine.cpp
    animations/ferenwidgetstatedata.cpp
    debug/ferenwidgetexplorer.cpp
    ferenaddeventfilter.cpp
    ferenblurhelper.cpp
    ferenframeshadow.cpp
    ferenhelper.cpp
    ferenmdiwindowshadow.cpp
    ferenmnemonics.cpp
    ferenpropertynames.cpp
    ferenshadowhelper.cpp
    ferensplitterproxy.cpp
    ferenstyle.cpp
    ferenstyleplugin.cpp
    ferentileset.cpp
    ferenwindowmanager.cpp
)

kconfig_add_kcfg_files(feren_PART_SRCS ferenconfigdata.kcfgc)
add_library(feren MODULE ${feren_PART_SRCS})
target_link_libraries(feren Qt5::Core Qt5::Gui Qt5::Widgets Qt5::DBus)
if( FEREN_HAVE_QTQUICK )
    target_link_libraries(feren Qt5::Quick)
endif()
target_link_libraries(feren KF5::ConfigCore KF5::ConfigWidgets KF5::GuiAddons KF5::IconThemes KF5::WindowSystem)
target_link_libraries(feren ferencommon5)

if(KF5FrameworkIntegration_FOUND)
    target_link_libraries(feren KF5::Style)
endif()

if (WIN32)
    # As stated in https://docs.microsoft.com/en-us/cpp/c-runtime-library/math-constants M_PI only gets defined
    # when if _USE_MATH_DEFINES is defined
    target_compile_definitions(feren PRIVATE _USE_MATH_DEFINES _BSD_SOURCE)
endif()

if(FEREN_HAVE_X11)
    target_link_libraries(feren ${XCB_LIBRARIES})
    target_link_libraries(feren Qt5::X11Extras)
endif()

if(FEREN_HAVE_KWAYLAND)
    target_link_libraries(feren KF5::WaylandClient)
endif()


########### install files ###############
install(TARGETS feren DESTINATION ${QT_PLUGIN_INSTALL_DIR}/styles/)
install(FILES feren.themerc  DESTINATION  ${DATA_INSTALL_DIR}/kstyle/themes)

########### subdirectories ###############
add_subdirectory(config)
