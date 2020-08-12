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

