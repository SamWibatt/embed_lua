# Sean wrote:
# Per 5.4 readme at https://www.lua.org/manual/5.4/readme.html
# library:
#     lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c lfunc.c lgc.c llex.c lmem.c 
#     lobject.c lopcodes.c lparser.c lstate.c lstring.c ltable.c ltm.c lundump.c 
#     lvm.c lzio.c lauxlib.c lbaselib.c lcorolib.c ldblib.c liolib.c lmathlib.c 
#     loadlib.c loslib.c lstrlib.c ltablib.c lutf8lib.c linit.c 

# I don't need the executables!
# interpreter:
#     library, lua.c 
# compiler:
#     library, luac.c 

cmake_minimum_required(VERSION 3.14)
add_library(lua STATIC
    src/lapi.c src/lcode.c src/lctype.c src/ldebug.c src/ldo.c src/ldump.c 
    src/lfunc.c src/lgc.c src/llex.c src/lmem.c src/lobject.c src/lopcodes.c 
    src/lparser.c src/lstate.c src/lstring.c src/ltable.c src/ltm.c src/lundump.c 
    src/lvm.c src/lzio.c src/lauxlib.c src/lbaselib.c src/lcorolib.c src/ldblib.c 
    src/liolib.c src/lmathlib.c src/loadlib.c src/loslib.c src/lstrlib.c src/ltablib.c 
    src/lutf8lib.c src/linit.c 
)
target_include_directories(lua PUBLIC src)