set(CMARK_TESTS OFF CACHE BOOL "Build cmark-gfm tests and enable testing" FORCE)
set(CMARK_STATIC ON CACHE BOOL "Build static libcmark-gfm library" FORCE)
set(CMARK_SHARED OFF CACHE BOOL "Build shared libcmark-gfm library" FORCE)
# EXCLUDE_FROM_ALL: do not install static libs
add_subdirectory(cmark-gfm-source EXCLUDE_FROM_ALL)

add_library(PluginCmarkGfm
    MODULE
        cmark-gfm-plugin.h
        cmark-gfm-plugin.cpp
)

add_dependencies(PluginCmarkGfm cmark-gfm)

target_include_directories(PluginCmarkGfm
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/cmark-gfm-source/src
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/cmark-gfm-source/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/cmark-gfm-source/extensions>
)

target_link_libraries(PluginCmarkGfm
    PRIVATE
        Qt5::Core
        libcmark-gfm-extensions_static
        libcmark-gfm_static
        # ask linker to help us finding unresolved symbols
        "-Wl,--no-undefined"
        # hide sundown internal symbols
        "-Wl,--exclude-libs,ALL"
)

install(TARGETS PluginCmarkGfm DESTINATION ${PLUGINPATH})
