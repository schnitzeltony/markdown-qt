cmake_minimum_required(VERSION 3.1)

project(markdown-qt LANGUAGES C CXX)
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
pkg_check_modules (CMARK REQUIRED libcmark>=0.28.3)

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

# all sundown headers are private
set(SUNDOWN_PRIVATE_HEADER_DIRS
    sundown/html
    sundown/src
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

# build the library
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${RESOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${MARKDOWNQT_VERSION_MAJOR})

# some target magic
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

# make linker happy
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Quick
    ${CMARK_LIBRARIES}
)

# tell cmake about public/privet headers
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE ${SUNDOWN_PRIVATE_HEADER_DIRS})

# install public header
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# install library
install(TARGETS ${PROJECT_NAME} EXPORT MarkdownQtConfig LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

# install cmake-files
install(EXPORT MarkdownQtConfig DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MarkdownQt)

# for builddir
export(TARGETS ${PROJECT_NAME} FILE MarkdownQtConfig.cmake)

# spawn out some info on configuration
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
