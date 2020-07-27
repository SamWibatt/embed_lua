cmake_minimum_required(VERSION 3.0.0)
project(luatest VERSION 0.1.0)

# not sure I want to do this like this
# from https://stackoverflow.com/questions/59436450/download-and-build-lua-with-modern-cmake -----------
# cmake_minimum_required(VERSION 3.10)
# include(ExternalProject)

# ExternalProject_Add(lua
#    URL "https://www.lua.org/ftp/lua-5.4.0.tar.gz"
#    CONFIGURE_COMMAND ""
#    BUILD_COMMAND make generic
#    BUILD_ALWAYS true
#    BUILD_IN_SOURCE true
#    INSTALL_COMMAND ""
# )
# add_library(liblua STATIC IMPORTED)
# ExternalProject_Get_property(lua SOURCE_DIR)
# message("liblua will be found at \'${SOURCE_DIR}/src\'")
# end from https://stackoverflow.com/questions/59436450/download-and-build-lua-with-modern-cmake

# just did it the dumb way and ran Make in lua-5.4.0 and liblua.a ends up in
# lua-5.4.0/src
# this makes that slightly less gross
set(LUA_DIR "lua-5.4.0/src")

include(CTest)
enable_testing()

add_executable(luatest main.cpp)
target_include_directories(luatest PUBLIC ${LUA_DIR})
target_link_directories(luatest PUBLIC ${LUA_DIR})
# dl avoids the link errors against dlopen, dlclose, etc.
target_link_libraries(luatest lua dl)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
