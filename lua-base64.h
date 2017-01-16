#ifndef LUA_BASE64
#define LUA_BASE64

#include <stdlib.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#define BASE64_LUA "lua-base64"

int luaopen_base64(lua_State *L);

#endif // LUA_BASE64
