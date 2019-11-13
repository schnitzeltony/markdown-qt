cmake_minimum_required(VERSION 3.1)

project(libmarkdown-qt LANGUAGES C CXX)
set(MARKDOWNQT_VERSION_MAJOR "0")
set(MARKDOWNQT_VERSION_MINOR "0")
set(MARKDOWNQT_VERSION_PATCH "1")

set(PROJECT_VERSION "${MARKDOWNQT_VERSION_MAJOR}.${MARKDOWNQT_VERSION_MINOR}.${MARKDOWNQT_VERSION_PATCH}")

# global Qt/C++ settings
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# plugin options
option(BUILD_CMARK_PLUGIN "Build cmark plugin" ON)
option(BUILD_CMARK_GFM_PLUGIN "Build cmark-gfm plugin" ON)
option(BUILD_DUMMY_PLUGIN "Build dummy plugin" ON)
option(BUILD_GITHUB_ONLINE_PLUGIN "Build github-online plugin" ON)
option(BUILD_HOEDOWN_PLUGIN "Build hoedown plugin" ON)
option(BUILD_QTWEBENGINEPDF_PLUGIN "Build qtwebenginepdf plugin" ON)
option(BUILD_SUNDOWN_PLUGIN "Build sundown plugin" ON)

# keep this just in case we get back to wkhtmltopdf or need other libs
# without build-system support
#set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake")

# cmake macros
include(FeatureSummary)
include(GNUInstallDirs)

# qt5 libs
find_package(Qt5 COMPONENTS Core Qml CONFIG REQUIRED)

add_subdirectory(src)

# install public headers
install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/markdown-qt
)

# spawn out some info on configuration
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
