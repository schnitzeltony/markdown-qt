add_library(PluginSundown
    SHARED
        sundown-plugin.h
        sundown-plugin.cpp
)

target_include_directories(PluginSundown
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/sundown-source/src
)

add_dependencies(PluginSundown Sundown)

target_link_libraries(PluginSundown
    PUBLIC
        Qt5::Core
    PRIVATE
        Sundown
        # ask linker to help us finding unresolved symbols
        "-Wl,--no-undefined"
        # hide sundown internal symbols
        "-Wl,--exclude-libs,ALL"
)

install(TARGETS PluginSundown DESTINATION ${PLUGINPATH})

# set sundown sources
set(SUNDOWN_SOURCES
    sundown-source/html/html.c
    sundown-source/html/html_smartypants.c
    sundown-source/html/houdini_html_e.c
    sundown-source/html/houdini_href_e.c
    sundown-source/src/autolink.c
    sundown-source/src/buffer.c
    sundown-source/src/markdown.c
    sundown-source/src/stack.c
)

add_library(Sundown
    STATIC
        ${SUNDOWN_SOURCES}
)

# seems that is not default for good old c
set_property(TARGET Sundown PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(Sundown
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/sundown-source/src
)

target_link_libraries(Sundown)

