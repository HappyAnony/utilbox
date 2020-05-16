# add the utilconf library
if(UTIL_CONF_ENABLE)
list(APPEND EXTRA_LIBS utilconf_static)
list(APPEND EXTRA_LIBS_DIRS "${PROJECT_BINARY_DIR}/source/conf")
endif()

# add the utilsqt  library
if(UTIL_SQT_ENABLE)
list(APPEND EXTRA_LIBS utilsqt_static)
list(APPEND EXTRA_LIBS_DIRS "${PROJECT_BINARY_DIR}/source/sqt")
endif()

# search source file list
utilbox_search_c_source_file("${PROJECT_SOURCE_DIR}/source" utilboxsrc)

# generate the executable file
add_executable(utilbox ${utilboxsrc})

# add search path for include files
target_include_directories(utilbox PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}"
                           "${PROJECT_SOURCE_DIR}/include")

# assign the library name required
target_link_libraries(utilbox PUBLIC ${EXTRA_LIBS})

# add search path for library files
target_link_directories(utilbox PUBLIC ${EXTRA_LIBS_DIRS})

