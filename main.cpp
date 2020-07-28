// learning to embed lua, per https://csl.name/post/lua-and-cpp/
// hm, getting link errors a la 
// https://stackoverflow.com/questions/23823031/trying-to-embed-lua-in-c-undefined-reference-to-dlopen
// but linking "dl" seems to have fixed it a la
// https://stackoverflow.com/questions/956640/linux-c-error-undefined-reference-to-dlopen
// k initial version works fine with hello.lua
// and the howdy callback works
// let's now look into the API https://www.lua.org/manual/5.2/manual.html#4

#include <stdio.h>

#ifdef __cplusplus
# include <lua.hpp>
#else
# include <lua.h>
# include <lualib.h>
# include <lauxlib.h>
#endif

//platform-specific
#include "plat_io.h"
#include "plat_luatest_main.h"

// let's see if I can load a lua script without just straight up running it, and call functions
// in it - the idea being that game actors would have certain interface functions - or we can
// discover what functions are available?
// http://gamedevgeek.com/tutorials/calling-lua-functions/
// looks like you run the program just the same, but all it has is function defs, then they're available
int luaadd (lua_State *L, int x, int y )
{
	int sum;

	/* the function name */
	int res = lua_getglobal(L, "add");

  // sean adds: check to see if that was successful - if not, bail with a 0
  // per https://www.lua.org/manual/5.4/manual.html#lua_getglobal, the return value is
  // "Returns the type of that value." looks like when it's loaded, res is 6 which is LUA_TFUNCTION
  if(res != LUA_TFUNCTION) {
      plprintf("'add' function isn't defined!\n");
      return 0;
  }

  plprintf("Add is defined! pushing x\n");

	/* the first argument */
	lua_pushnumber(L, x);

  plprintf("Pushing y\n");
	/* the second argument */
	lua_pushnumber(L, y);

  plprintf("Doing lua call\n");
	/* call the function with 2 arguments, return 1 result */
	lua_call(L, 2, 1);

	/* get the result */
  plprintf("Getting result\n");
	sum = (int)lua_tointeger(L, -1);

  plprintf("Cleaning up lua stack - sum is %d\n",sum);
	lua_pop(L, 1);

	return sum;
}



// callback from https://csl.name/post/lua-and-cpp/ ============================================
// for lua calling into C; registered below after the Lua state is created.
#ifdef __cplusplus
extern "C"
#endif
int howdy(lua_State* state)
{
  // The number of function arguments will be on top of the stack.
  int args = lua_gettop(state);

  plprintf("howdy() was called with %d arguments:\n", args);

  // on arduino this messes up a bit, numeric argument came back as 'ld'
  for ( int n=1; n<=args; ++n) {
    plprintf("  argument %d: '%s'\n", n, lua_tostring(state, n));
  }

  // Push the return value on top of the stack. NOTE: We haven't popped the
  // input arguments to our function. To be honest, I haven't checked if we
  // must, but at least in stack machines like the JVM, the stack will be
  // cleaned between each function call.
  // FIND OUT IF WE NEED TO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // per https://www.lua.org/manual/5.2/manual.html#4,
  // "Whenever Lua calls C, the called function gets a new stack, which is independent of previous 
  // stacks and of stacks of C functions that are still active. This stack initially contains any 
  // arguments to the C function and it is where the C function pushes its results to be returned 
  // to the caller (see lua_CFunction)."
  // so it doesn't look like we need to do any cleaning up? Though investigate lua_checkstack

  lua_pushnumber(state, 123);

  // Let Lua know how many return values we've passed
  return 1;
}

// basic lua integration stuff ==================================================

void print_error(lua_State* state) {
  // The error message is on top of the stack.
  // Fetch it, print it and then pop it off the stack.
  const char* message = lua_tostring(state, -1);
  plprintf(message);
  lua_pop(state, 1);
}

void execute(const char* code)
{
  lua_State *state = luaL_newstate();

  // Make standard libraries available in the Lua object
  luaL_openlibs(state);

  // Make howdy() available to Lua programs under the same name.
  lua_register(state, "howdy", howdy);


  int result;

  // Load the program; this supports both source code and bytecode files.
  // sean changes from a filename to code we'll directly run
  result = luaL_loadstring(state, code);

  if ( result != LUA_OK ) {
    print_error(state);
    return;
  }

  // Finally, execute the program by calling into it.
  // Change the arguments if you're not running vanilla Lua code.

  result = lua_pcall(state, 0, LUA_MULTRET, 0);

  if ( result != LUA_OK ) {
    print_error(state);
    return;
  }

  // Try calling the add function
  plprintf("Calling lua add function with 5 and 2\n");
  int res = luaadd(state, 5, 2);
  plprintf("Result is %d\n",res);
}

// main is now inside the platform-specfic dirs
//int main(int argc, char** argv)

const char *lua_program1 = "io.write(string.format(\"Hello from %s\\n\", _VERSION))";
const char *lua_program2 = 
  "io.write(\"Calling howdy() ...\\n\")\nlocal value = howdy(\"First\", \"Second\", 112233)\nio.write(string.format(\"howdy() returned: %s\\n\", tostring(value)))";
const char *lua_program3 = "function add ( x, y )\nreturn x + y\nend";

void run()
{
  // if ( argc <= 1 ) {
  //   puts("Usage: luatest file(s)");
  //   puts("Loads and executes Lua programs.");
  //   return 1;

  //   //temp: just run "hello.lua"
  //   // execute("hello.lua");
  //   //execute("add.lua");
  // }

  // // Execute all programs on the command line
  // for ( int n=1; n<argc; ++n ) {
  //   execute(argv[n]);
  // }

  // return 0;

  plprintf("===============================================\n");
  execute(lua_program1);
  plprintf("===============================================\n");
  // looks like "howdy" makes it hang after hello
  execute(lua_program2);
  plprintf("===============================================\n");
  // so does add... or is it just that running more than 1 program kills it? running ONLY add works - running ONLY howdy gets as far as howdy printing its args
  // result is wrong but add does call and return...
  // ===============================================
  // Calling lua add function with 5 and 2
  // Add is defined! pushing x
  // Pushing y
  // Doing lua call
  // Getting result
  // Cleaning up lua stack - sum is 0
  // Result is 0
  // ===============================================  
  execute(lua_program3);
  plprintf("===============================================\n");
}
