find_package(Qt5 COMPONENTS WebEngine WebEngineWidgets PrintSupport CONFIG REQUIRED)

add_library(PluginQtWebEnginePdf
    MODULE
        qtwebenginepdf-plugin.h
        qtwebenginepdf-plugin.cpp
)

target_include_directories(PluginQtWebEnginePdf
    PRIVATE
)

target_link_libraries(PluginQtWebEnginePdf
    PRIVATE
        Qt5::Core
        Qt5::WebEngine
        Qt5::WebEngineWidgets
        Qt5::PrintSupport
        MarkdownQt
        # ask linker to help us finding unresolved symbols
        "-Wl,--no-undefined"
)

install(TARGETS PluginQtWebEnginePdf DESTINATION ${PLUGINPATH})

