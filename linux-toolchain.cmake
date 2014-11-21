SET (CMAKE_C_COMPILER gcc)
SET (CMAKE_C_FLAGS "-Wall -g -O2 -std=c99 -Wno-sign-compare" CACHE STRING "-Wall -g -O2 -std=c99 -Wno-sign-compare")
SET (CMAKE_C_FLAGS_RELEASE "-Wall -O2 -std=c99 -Wno-sign-compare" CACHE STRING "-Wall -O2 -std=c99 -Wno-sign-compare")
 
SET (CMAKE_CXX_COMPILER g++)
SET (CMAKE_CXX_FLAGS "-Wall -g -mavx -std=c++11 -Wno-sign-compare" CACHE STRING "-Wall -g -O2 -std=c++11 -Wno-sign-compare")
SET (CMAKE_CXX_FLAGS_RELEASE "-Wall -DNDEBUG -O2 -std=c++11" CACHE STRING "release build")
SET (CMAKE_CXX_FLAGS_DEBUG "-Wall -mavx -g -std=c++11 -Wno-sign-compare" CACHE STRING "debug build")

SET (CMAKE_EXPORT_COMPILE_COMMANDS 1)
SET (CMAKE_EXPORT_COMPILE_COMMANDS "${CMAKE_EXPORT_COMPILE_COMMANDS}" CACHE STRING "export compile_commands.json")
