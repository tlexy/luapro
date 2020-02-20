#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <string>
#include <cstring>

using namespace luabridge;

typedef struct _Lua
{
    lua_State* L;
}Lua;

Lua* proCreate();

void proClose(Lua* lua);

//加载lua文件
bool proLoadLuaFile(Lua* lua, const char* file);

//导出C函数
#define expCFunction(name, func, lvm) \
    getGlobalNamespace(lvm->L).addFunction(name, func)

//注册类1
#define regClass(cls) \
	getGlobalNamespace(lvm->L).beginClass<cls>(#cls).endClass();

//注册类2
#define expClass(cls, name, ctor) \
	getGlobalNamespace(lvm->L).beginClass<cls>(name).addConstructor<ctor>().endClass();

//导出一般类方法
#define expClassFunction(cls, name, func, func_alias) \
	getGlobalNamespace(lvm->L).beginClass<cls>(name).addFunction(func_alias, &cls::func).endClass();

//导出静态类方法
#define expClassSFunction(cls, name, func, func_alias) \
	getGlobalNamespace(lvm->L).beginClass<cls>(name).addStaticFunction(func_alias, &cls::func).endClass();

void luaPush(Lua* L, int val);

void luaPush(Lua* L, unsigned val);

void luaPush(Lua* L, short val);

void luaPush(Lua* L, unsigned short val);

void luaPush(Lua* L, long val);

void luaPush(Lua* L, unsigned long val);

void luaPush(Lua* L, long long val);

void luaPush(Lua* L, long long unsigned val);

void luaPush(Lua* L, char val);

void luaPush(Lua* L, unsigned char val);

void luaPush(Lua* L, bool val);
///---
void luaPush(Lua* L, const std::string& val);

void luaPush(Lua* L, const char* val);

template <typename V>
void expVal(const char* name, V val, Lua* lua)
{
    luaPush(lua, val);
    lua_setglobal(lua->L, name);
}
