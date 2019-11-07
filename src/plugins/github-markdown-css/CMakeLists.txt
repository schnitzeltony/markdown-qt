add_library(PluginGithubMarkdownCss
    SHARED
        github-markdown-css-plugin.h
        github-markdown-css-plugin.cpp
        resources.qrc
)

target_link_libraries(PluginGithubMarkdownCss
    PRIVATE
        Qt5::Core
)

install(TARGETS PluginGithubMarkdownCss DESTINATION ${PLUGINPATH})
