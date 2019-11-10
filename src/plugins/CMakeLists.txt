if(BUILD_CMARK_PLUGIN)
add_subdirectory(cmark)
endif(BUILD_CMARK_PLUGIN)

if(BUILD_CMARK_GFM_PLUGIN)
add_subdirectory(cmark-gfm)
endif(BUILD_CMARK_GFM_PLUGIN)

if(BUILD_DUMMY_PLUGIN)
add_subdirectory(dummy)
endif(BUILD_DUMMY_PLUGIN)

add_subdirectory(github-markdown-css)

if(BUILD_HOEDOWN_PLUGIN)
add_subdirectory(hoedown)
endif(BUILD_HOEDOWN_PLUGIN)

if(BUILD_SUNDOWN_PLUGIN)
add_subdirectory(sundown)
endif(BUILD_SUNDOWN_PLUGIN)

