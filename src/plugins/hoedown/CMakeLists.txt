add_library(PluginHoedown
    MODULE
        hoedown-plugin.h
        hoedown-plugin.cpp
)

target_include_directories(PluginHoedown
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/hoedown-source/src
)

add_dependencies(PluginHoedown Hoedown)

target_link_libraries(PluginHoedown
    PRIVATE
        Qt5::Core
        Hoedown
        MarkdownQt
        # ask linker to help us finding unresolved symbols
        "-Wl,--no-undefined"
)

install(TARGETS PluginHoedown DESTINATION ${PLUGINPATH})

# set hoedown sources
set(HOEDOWN_SOURCES
    hoedown-source/src/autolink.c
    hoedown-source/src/buffer.c
    hoedown-source/src/document.c
    hoedown-source/src/escape.c
    hoedown-source/src/html.c
    hoedown-source/src/html_blocks.c
    hoedown-source/src/html_smartypants.c
    hoedown-source/src/stack.c
    hoedown-source/src/version.c
)

add_library(Hoedown
    STATIC
        ${HOEDOWN_SOURCES}
)

# seems that is not default for good old c
set_property(TARGET Hoedown PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(Hoedown
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/hoedown-source/src
)

target_link_libraries(Hoedown)

