add_library(PluginDummy
    MODULE
        dummy-plugin.h
        dummy-plugin.cpp
)

target_link_libraries(PluginDummy
    PRIVATE
        Qt5::Core
        MarkdownQt
        # ask linker to help us finding unresolved symbols
        "-Wl,--no-undefined"
)

install(TARGETS PluginDummy DESTINATION ${PLUGINPATH})
