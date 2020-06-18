################# dependencies #################
### Qt/KDE
find_package(Qt5 REQUIRED CONFIG COMPONENTS Widgets)

################# ferenstyle target #################
set(ferencommon_LIB_SRCS
    ferenboxshadowrenderer.cpp
)

add_library(ferencommon5 ${ferencommon_LIB_SRCS})

generate_export_header(ferencommon5
    BASE_NAME ferencommon
    EXPORT_FILE_NAME ferencommon_export.h)

target_link_libraries(ferencommon5
    PUBLIC
        Qt5::Core
        Qt5::Gui)

set_target_properties(ferencommon5 PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR})

install(TARGETS ferencommon5 ${INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)
