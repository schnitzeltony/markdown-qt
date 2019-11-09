find_package(PkgConfig REQUIRED)
pkg_check_modules(CMARK REQUIRED libcmark>=0.28.3)

add_library(PluginCmark
    MODULE
        cmark-plugin.h
        cmark-plugin.cpp
)

target_include_directories(PluginCmark
    PRIVATE
)

target_link_libraries(PluginCmark
    PRIVATE
        Qt5::Core
        ${CMARK_LIBRARIES}
        MarkdownQt
)

install(TARGETS PluginCmark DESTINATION ${PLUGINPATH})

