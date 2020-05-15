find_package(LibJsonc REQUIRED)

# search source file list
utilbox_search_c_source_file("${PROJECT_SOURCE_DIR}/source/conf" utilconfsrc)

# generate static and shared library target
add_library(utilconf_static STATIC ${utilconfsrc})
add_library(utilconf_shared SHARED ${utilconfsrc})

# add search path for static library include files
target_include_directories(utilconf_static PUBLIC
                           "${PROJECT_SOURCE_DIR}"
                           "${PROJECT_SOURCE_DIR}/include"
                           "${LIBJOSNC_INCLUDE_DIRS}"
                           )

# add search path for shared library include files
target_include_directories(utilconf_shared PUBLIC
                           "${PROJECT_SOURCE_DIR}"
                           "${PROJECT_SOURCE_DIR}/include"
                           "${LIBJSONC_INCLUDE_DIRS}"
                           )

# assign the library name required
target_link_libraries(utilconf_static PUBLIC ${LIBJSONC_LIBRARY_NAME})
#target_link_libraries(utilconf_shared PUBLIC "json-c"})

# add search path for library files
target_link_directories(utilconf_static PUBLIC ${LIBJSONC_LIBRARY_DIRS})
target_link_directories(utilconf_shared PUBLIC ${LIBJSONC_LIBRARY_DIRS})

# set library output name
set_target_properties(utilconf_static PROPERTIES OUTPUT_NAME utilconf)
set_target_properties(utilconf_shared PROPERTIES OUTPUT_NAME utilconf)

set_target_properties(utilconf_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(utilconf_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# set utilconf version
set(UTILCONF_VERSION 0.5)

# set shared library version, will generate libutilconf.${UTILCONF_VERSION}.so 
# and a symbol link named libutilconf.so
set_target_properties(utilconf_shared PROPERTIES VERSION ${UTILCONF_VERSION} SOVERSION ${UTILCONF_VERSION})
