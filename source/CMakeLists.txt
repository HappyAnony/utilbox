# set utilbox's options
option(UTIL_CONF_ENABLE  "Use utilbox provided conf implementation" ON)
option(UTIL_SQT_ENABLE   "Use utilbox provided sqt implementation" OFF)

#configure a header file to pass some of the CMake settings to the source code
configure_file(utilboxConfig.h.in utilboxConfig.h @ONLY)

# add the utilconf library
if(UTIL_CONF_ENABLE)
list(APPEND EXTRA_LIBS utilconf)
endif()

# add the utilsqt  library
if(UTIL_SQT_ENABLE)
list(APPEND EXTRA_LIBS utilsqt)
endif()

# add source files
set(utilboxsrc
    utilbox.c)

# generate the executable file
add_executable(utilbox ${utilboxsrc})

# add search path for shared library include files
target_include_directories(utilbox PUBLIC
                           "${PROJECT_SOURCE_DIR}"
                           "${PROJECT_SOURCE_DIR}/include")

# assign the library name required
target_link_libraries(utilbox PUBLIC ${EXTRA_LIBS})
target_link_directories(utilbox PUBLIC "${PROJECT_BINARY_DIR}/source/conf")

