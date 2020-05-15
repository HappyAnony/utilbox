# - Try to find LibJsonc
# Once done this will define
#
#   LIBJSONC_FOUND - system has LibJsonc
#   LIBJSONC_INCLUDE_DIRS - the LibJsonc include directory
#   LIBJSONC_LIBRARY_DIRS - the LibJsonc library directory
#   LIBJSONC_LIBRARY_NAME - the LibJsonc library name
#
# Copyright (c) 2020-2030, HappyAnony <13545108786@163.com>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#   * Redistributions of source code must retain the above copyright notice,
#     this list of conditions and the following disclaimer.
#   * Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#   * Neither the name of Redis nor the names of its contributors may be used
#     to endorse or promote products derived from this software without
#     specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

if (LIBJSONC_INCLUDE_DIRS AND LIBJSONC_LIBRARY_DIRS)
    # already in cache
    set (LIBJSONC_FOUND TRUE)
else (LIBJSONC_INCLUDE_DIRS AND LIBJSONC_LIBRARY_DIRS)
    
    find_path(LIBJSONC_INCLUDE_DIRS
      NAMES
        json-c/json.h
        json-c/json_object.h
        json-c/json_object_private.h
        json-c/json_util.h
      PATHS
        /usr/include
        /usr/local/include
        /opt/local/include
        ${CMAKE_INCLUDE_PATH}
        ${CMAKE_INSTALL_PREFIX}/include
    )

    find_library(LIBJSONC_LIBRARY
      NAMES
        libjson-c.so
      PATHS
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        ${CMKAE_LIBRARY_PATH}
        ${CMAKE_INSTALL_PREFIX}/lib
    )

    if (LIBJSONC_INCLUDE_DIRS AND LIBJSONC_LIBRARY)
        set(LIBJSONC_FOUND TRUE)
    else (LIBJSONC_INCLUDE_DIRS AND LIBJSONC_LIBRARY)
        set(LIBJSONC_FOUND FALSE)
    endif (LIBJSONC_INCLUDE_DIRS AND LIBJSONC_LIBRARY)

    if (LIBJSONC_FOUND)
        string(REPLACE "libjson-c.so" ""
          LIBJSONC_LIBRARY_DIRS
          ${LIBJSONC_LIBRARY}
        )

        set(LIBJSONC_LIBRARY_NAME "json-c")
    endif (LIBJSONC_FOUND)

endif (LIBJSONC_INCLUDE_DIRS AND LIBJSONC_LIBRARY_DIRS) 

message(STATUS "LIBJSONC_FOUND       : ${LIBJSONC_FOUND}       ")
message(STATUS "LIBJSONC_INCLUDE_DIRS: ${LIBJSONC_INCLUDE_DIRS}") 
message(STATUS "LIBJSONC_LIBRARY_DIRS: ${LIBJSONC_LIBRARY_DIRS}") 

if (NOT LIBJSONC_FOUND)
    message(FATAL_ERROR "json-c library not found") 
endif(NOT LIBJSONC_FOUND)

