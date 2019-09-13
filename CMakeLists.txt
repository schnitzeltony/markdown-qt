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

# Qt5 libs
find_package(Qt5 COMPONENTS Core Quick REQUIRED)

# cmark lib
find_package(PkgConfig REQUIRED)
pkg_check_modules (CMARK REQUIRED libcmark>=0.28.3)

# all sources by type
set(PUBLIC_HEADERS
    cmarkdown-qt.h
)

include_directories(
    ${CMAKE_SOURCE_DIR}/sundown/src
)

set(SOURCES_SUNDOWN
    sundown/html/html.c
    sundown/html/html_smartypants.c
    sundown/html/houdini_html_e.c
    sundown/html/houdini_href_e.c
    sundown/src/autolink.c
    sundown/src/buffer.c
    sundown/src/markdown.c
    sundown/src/stack.c
)

set(SOURCES
    cmarkdown-qt.cpp
    ${SOURCES_SUNDOWN}
)

set(RESOURCES
    resources.qrc
)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${RESOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${MARKDOWNQT_VERSION_MAJOR})
#set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER ${PUBLIC_HEADERS})

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Quick
    ${CMARK_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})

