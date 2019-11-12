find_path(WKHTMLTOPDF_INCLUDE_DIR pdf.h
    PATH_SUFFIXES include/wkhtmltox include/wkhtmltopdf include
)

message("WkHtmlToPdf include dir: ${WKHTMLTOPDF_INCLUDE_DIR}")

find_library(WKHTMLTOPDF_LIBRARY
    NAMES wkhtmltox
    PATH_SUFFIXES lib
)

message("WkHtmlToPdf library: ${WKHTMLTOPDF_LIBRARY}")

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(WkHtmlToPdf DEFAULT_MSG WKHTMLTOPDF_LIBRARY WKHTMLTOPDF_INCLUDE_DIR)
mark_as_advanced(WKHTMLTOPDF_LIBRARY WKHTMLTOPDF_INCLUDE_DIR)
