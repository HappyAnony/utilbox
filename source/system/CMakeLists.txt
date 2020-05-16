# search source file list
utilbox_search_c_source_file("${PROJECT_SOURCE_DIR}/source/system" utilsystemsrc)

# generate static and shared library target
add_library(utilsystem_static STATIC ${utilsystemsrc})
add_library(utilsystem_shared SHARED ${utilsystemsrc})

# add search path for static library include files
target_include_directories(utilsystem_static PUBLIC
                           "${PROJECT_SOURCE_DIR}"
                           "${PROJECT_SOURCE_DIR}/include"
                           )

# add search path for shared library include files
target_include_directories(utilsystem_shared PUBLIC
                           "${PROJECT_SOURCE_DIR}"
                           "${PROJECT_SOURCE_DIR}/include"
                           )

# set library output name
set_target_properties(utilsystem_static PROPERTIES OUTPUT_NAME utilsystem)
set_target_properties(utilsystem_shared PROPERTIES OUTPUT_NAME utilsystem)

set_target_properties(utilsystem_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(utilsystem_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# set utilsystem version
set(UTILSYSTEM_VERSION 0.5)

# set shared library version, will generate libutilsystem.${UTILSYSTEM_VERSION}.so 
# and a symbol link named libutilsystem.so
set_target_properties(utilsystem_shared PROPERTIES VERSION ${UTILSYSTEM_VERSION} SOVERSION ${UTILSYSTEM_VERSION})
