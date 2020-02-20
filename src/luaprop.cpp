#include "luaprop.h"
#include <cstdlib>

Lua* proCreate()
{
    Lua* lua = new Lua;
    lua->L = luaL_newstate();
    if (lua->L == NULL)
    {
        return NULL;
    }
    luaL_openlibs(lua->L);
    return lua;
}

void proClose(Lua* lua)
{
    lua_close(lua->L);
    free(lua);
}

bool proLoadLuaFile(Lua* lua, const char* file)
{
	int flag = luaL_loadfile(lua->L, file);
	if (flag != LUA_OK)
	{
		//if (flag == LUA_ERRSYNTAX)
		//{
		fprintf(stderr, "\nFATAL ERROR:%s\n\n", lua_tostring(lua->L, -1));
		//}
		std::cout << "load lua file error, errno = " << flag << std::endl;
		return false;
	}
	int ret = lua_pcall(lua->L, 0, -1, 0);
	if (ret != LUA_OK)
	{
		fprintf(stderr, "\nFATAL ERROR, ERRNO, ERRTIPS:%d\n%s\n\n", ret, lua_tostring(lua->L, -1));
		return false;
	}
	return true;
}

void luaPush(Lua* lua, int val)
{
	lua_pushinteger(lua->L, val);
}

void luaPush(Lua* lua, unsigned val)
{
	lua_pushinteger(lua->L, val);
}

void luaPush(Lua* lua, short val)
{
	lua_pushinteger(lua->L, val);
}

void luaPush(Lua* lua, unsigned short val)
{
	lua_pushinteger(lua->L, val);
}

void luaPush(Lua* lua, long val)
{
	lua_pushinteger(lua->L, val);
}

void luaPush(Lua* lua, unsigned long val)
{
	lua_pushinteger(lua->L, val);
}

void luaPush(Lua* lua, long long val)
{
	lua_pushinteger(lua->L, val);
}

void luaPush(Lua* lua, long long unsigned val)
{
	lua_pushinteger(lua->L, val);
}

void luaPush(Lua* lua, char val)
{
	lua_pushinteger(lua->L, val);
}

void luaPush(Lua* lua, unsigned char val)
{
	lua_pushinteger(lua->L, val);
}

void luaPush(Lua* lua, bool val)
{
	lua_pushboolean(lua->L, val ? 1 : 0);
}
///---
void luaPush(Lua* lua, const std::string& val)
{
	lua_pushstring(lua->L, val.c_str());
}

void luaPush(Lua* lua, const char* val)
{
	lua_pushstring(lua->L, val);
}
