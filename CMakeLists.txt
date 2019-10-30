cmake_minimum_required(VERSION 3.1)

project(libmarkdownqt LANGUAGES C CXX)
set(MARKDOWNQT_VERSION_MAJOR "0")
set(MARKDOWNQT_VERSION_MINOR "0")
set(MARKDOWNQT_VERSION_PATCH "1")

set(PROJECT_VERSION "${MARKDOWNQT_VERSION_MAJOR}.${MARKDOWNQT_VERSION_MINOR}.${MARKDOWNQT_VERSION_PATCH}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# cmake macros
include(FeatureSummary)
include(GNUInstallDirs)

# qt5 libs
find_package(Qt5 COMPONENTS Core Quick REQUIRED)

# cmark lib
find_package(PkgConfig REQUIRED)
pkg_check_modules(CMARK REQUIRED libcmark>=0.28.3)

# all sources by type
set(PUBLIC_HEADERS
    markdown-qt.h
)

# find sundown includes
include_directories(
    ${CMAKE_SOURCE_DIR}/sundown/src
)

# set sundown sources
set(SUNDOWN_SOURCES
    sundown/html/html.c
    sundown/html/html_smartypants.c
    sundown/html/houdini_html_e.c
    sundown/html/houdini_href_e.c
    sundown/src/autolink.c
    sundown/src/buffer.c
    sundown/src/markdown.c
    sundown/src/stack.c
)

# all sources (once support for installed sundown is added this needs change)
set(SOURCES
    src/markdown-qt.cpp
    ${SUNDOWN_SOURCES}
)

# md templates as resources
set(RESOURCES
    resources.qrc
)

# the library
add_library(MarkdownQt
    SHARED
        ${SOURCES} ${RESOURCES}
)
set_target_properties(MarkdownQt PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(MarkdownQt PROPERTIES SOVERSION ${MARKDOWNQT_VERSION_MAJOR})

# some target magic
target_compile_definitions(MarkdownQt PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

# dependencies
target_link_libraries(MarkdownQt
    PUBLIC
        Qt5::Core
        Qt5::Quick
    PRIVATE
        ${CMARK}
)

# tell cmake about public/private headers
target_include_directories(MarkdownQt
    PUBLIC
        $<INSTALL_INTERFACE:include/markdown-qt>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/sundown/html
        ${CMAKE_CURRENT_SOURCE_DIR}/sundown/src
)

# compiler options
target_compile_features(MarkdownQt PRIVATE cxx_std_11)
target_compile_options(MarkdownQt PRIVATE -Wall)

# install public header
install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# install library
install(TARGETS MarkdownQt
    EXPORT MarkdownQtExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# configure MarkdownQtConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    MarkdownQtConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMinorVersion
)

# configure MarkdownQtConfig.cmake
configure_file(MarkdownQtConfig.cmake.in MarkdownQtConfig.cmake @ONLY)

# configure MarkdownQtConfig(Version).cmake
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/MarkdownQtConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/MarkdownQtConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MarkdownQt
)

# install targets cmake-files
install(EXPORT MarkdownQtExport
    FILE MarkdownQtTargets.cmake
    NAMESPACE MarkdownQt
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MarkdownQt
)


# spawn out some info on configuration
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
