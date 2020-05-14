# set c make flags
set(CMAKE_C_FLAGS         "${CMAKE_C_FLAGS} -Werror -Wall -Wextra -fvisibility=hidden -Wno-format-zero-length -Wno-format-security -gdwarf-2 -g3")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_C_FLAGS_PACKAGE "-g -O2 -DNDEBUG")
set(CMAKE_C_FLAGS_DEBUG   "-g -O0")

