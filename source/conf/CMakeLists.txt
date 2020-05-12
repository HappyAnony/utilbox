# set source files
set(src
    util_json.c
    util_json_parser.c)

# set head files
set(inc
    util_json.h
    util_json_parser.h)

# add static and shared library target
add_library(utilconf_static STATIC ${src})
add_library(utilconf_shared SHARED ${src})

# add search path for include files
target_include_directories(utilconf_static PUBLIC
                           "${PROJECT_SOURCE_DIR}"
                           "${PROJECT_SOURCE_DIR}/include"
                           )

# add search path for include files
target_include_directories(utilconf_shared PUBLIC
                           "${PROJECT_SOURCE_DIR}"
                           "${PROJECT_SOURCE_DIR}/include"
                           )

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
