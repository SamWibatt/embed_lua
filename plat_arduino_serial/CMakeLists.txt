add_library(PlatLuatestMain plat_luatest_main.cpp)
target_link_arduino_libraries(PlatLuatestMain PRIVATE core)

add_library(PlatIO plat_io.cpp)
target_link_arduino_libraries(PlatIO PRIVATE core)
