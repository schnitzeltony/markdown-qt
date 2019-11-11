find_package(Qt5 COMPONENTS Network CONFIG REQUIRED)

add_library(PluginGithubOnline
    MODULE
        github-online-plugin.h
        github-online-plugin.cpp
)

add_dependencies(PluginGithubOnline cmark-gfm)

target_include_directories(PluginGithubOnline
    PRIVATE
)

target_link_libraries(PluginGithubOnline
    PRIVATE
        Qt5::Core
        Qt5::Network
        MarkdownQt
        # ask linker to help us finding unresolved symbols
        "-Wl,--no-undefined"
)

install(TARGETS PluginGithubOnline DESTINATION ${PLUGINPATH})
