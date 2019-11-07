# all sources (once support for installed sundown is added this needs change)

set(PUBLIC_HEADERS
    ${CMAKE_SOURCE_DIR}/include/markdown-qt.h
)

set(PRIVATE_HEADERS
)

set(SOURCES
    markdown-qt.cpp
)

# md templates as resources
set(RESOURCES
    resources.qrc
)

# the library
add_library(MarkdownQt
    SHARED
        ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES} ${RESOURCES}
)
add_library(MarkdownQt::MarkdownQt ALIAS MarkdownQt)
set_target_properties(MarkdownQt PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(MarkdownQt PROPERTIES SOVERSION ${MARKDOWNQT_VERSION_MAJOR})


# export header
include(GenerateExportHeader)
generate_export_header(MarkdownQt)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/markdownqt_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/markdown-qt
)


# tell all about public/private headers
include_directories(
    # public header
    $<INSTALL_INTERFACE:include/markdown-qt>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    # generated export header
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    # plugin-interface header
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# compiler options
target_compile_features(MarkdownQt PRIVATE cxx_std_11)
target_compile_options(MarkdownQt PRIVATE -Wall)

# some target magic added by qtcreator
target_compile_definitions(MarkdownQt PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(MarkdownQt
    PRIVATE
        Qt5::Core
        Qt5::Qml
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

# install MarkdownQtConfig(Version).cmake
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/MarkdownQtConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/MarkdownQtConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MarkdownQt
)

# install targets cmake-files
install(EXPORT MarkdownQtExport
    FILE MarkdownQtTargets.cmake
    NAMESPACE MarkdownQt::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MarkdownQt
)


add_subdirectory(plugins)

