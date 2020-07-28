cmake_minimum_required(VERSION 3.14.0)    # arduino stuff needs some recent cmake
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
# set(LUA_DIR "lua-5.4.0/src")

# include(CTest)
# enable_testing()

# let's try it pure-cmake

add_executable(luatest main.cpp)

# platform specific libraries and includes 
if(TARGET_PLATFORM MATCHES "Arduino*")
    if(TARGET_PLATFORM STREQUAL "ArduinoSerial")
        message("Adding plat_arduino_serial subdirectory & includes")
        add_subdirectory("${PROJECT_SOURCE_DIR}/plat_arduino_serial")
        list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/plat_arduino_serial")
    endif()
    # make lua all 32-bit
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DLUA_32BITS")
    target_link_arduino_libraries(luatest PRIVATE core)
    # OTHER LIBRARIES LIKE MCUFRIEND OR WHATEVER GO HERE 
    # target for making it generate hex 
    target_enable_arduino_upload(luatest)
elseif(TARGET_PLATFORM MATCHES "Linux*")
    #add the dl library 
    list(APPEND EXTRA_LIBS dl)
    if(TARGET_PLATFORM STREQUAL "LinuxConsole")
        add_subdirectory("${PROJECT_SOURCE_DIR}/plat_linux_console")
        message("Hey about to add plat linux console")
        list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/plat_linux_console")
    endif()
endif()

# MAKE THIS MORE VISIBLE
set(LUA_DIR "lua-5.4.0")
list(APPEND EXTRA_INCLUDES "${LUA_DIR}/src")      # is this how you do this?
add_subdirectory(lua-5.4.0)

# add the platspec libraries 
list(APPEND EXTRA_LIBS PlatIO)

# and the lua library
list(APPEND EXTRA_LIBS lua)

# target_include_directories(luatest PUBLIC ${LUA_DIR})
# target_link_directories(luatest PUBLIC ${LUA_DIR})
# dl avoids the link errors against dlopen, dlclose, etc.
# can avoid the need for it if I change luaconf.h, qv; find out what dlopen is for
# if it's for dynamic linking, I can avoid
target_link_libraries(luatest PUBLIC ${EXTRA_LIBS})
target_link_libraries(luatest PUBLIC PlatLuatestMain)     # don't forget the main!
target_include_directories(luatest PUBLIC ${EXTRA_INCLUDES})

if(TARGET_PLATFORM MATCHES "Arduino*")
  add_custom_command(TARGET luatest POST_BUILD
    USES_TERMINAL
    VERBATIM
    COMMAND echo "**********************************************************************************************"
    COMMAND echo "**********************************************************************************************"
    COMMAND echo "**********************************************************************************************"
    COMMAND echo ".bin and .hex files have been emitted for luatest"
    COMMAND echo "Assuming a SAMD21 target, you can burn with bossac installed with  e.g." 
    COMMAND echo "~/.arduino15/packages/arduino/tools/bossac/1.7.0-arduino3/bossac -i -d --port=<device port> -U true -i -e -w -v luatest.bin -R"
    COMMAND echo "**********************************************************************************************"
    COMMAND echo "**********************************************************************************************"
    COMMAND echo "**********************************************************************************************"
  )
endif()


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
