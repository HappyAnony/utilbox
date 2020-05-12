# set utilbox's options
option(UTIL_CONF_ENABLE  "Use utilbox provided conf implementation" ON)
option(UTIL_SQT_ENABLE   "Use utilbox provided sqt implementation" OFF)

#configure a header file to pass some of the CMake settings to the source code
configure_file(utilboxConfig.h.in utilboxConfig.h @ONLY)


# add the utilconf library
if(UTIL_CONF_ENABLE)
#add_subdirectory(conf)
list(APPEND EXTRA_LIBS utilconf)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/include/conf")
endif()

# add the utilsqt  library
if(UTIL_SQT_ENABLE)
add_subdirectory(sqt)
list(APPEND EXTRA_LIBS utilsqt)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/include/sqt")
endif()

# add source files
set(utilboxsrc
    utilbox.c)

# generate the executable file
add_executable(utilbox ${utilboxsrc})

# assign the library name required
target_link_libraries(utilbox PUBLIC ${EXTRA_LIBS})

