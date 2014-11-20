SET (CMAKE_C_COMPILER cc)
SET (CMAKE_C_FLAGS "-Wall -g -O2 -std=c99 -Wno-sign-compare" CACHE STRING "-Wall -g -O2 -std=c99 -Wno-sign-compare")
SET (CMAKE_CXX_FLAGS_RELEASE "-Wall -O2 -std=c++11 -Wno-sign-compare" CACHE STRING "-Wall -O2 -std=c++11 -Wno-sign-compare")

SET (CMAKE_CXX_COMPILER c++)
SET (CMAKE_CXX_FLAGS "-Wall -g -O2 -std=c++11 -Wno-sign-compare" CACHE STRING "-Wall -g -O2 -std=c++11 -Wno-sign-compare")
SET (CMAKE_CXX_FLAGS_RELEASE "-Wall -O2 -std=c++11 -Wno-sign-compare" CACHE STRING "-Wall -O2 -std=c++11 -Wno-sign-compare")

SET (CMAKE_EXPORT_COMPILE_COMMANDS 1)
SET (CMAKE_EXPORT_COMPILE_COMMANDS "${CMAKE_EXPORT_COMPILE_COMMANDS}" CACHE STRING "export compile_commands.json")
