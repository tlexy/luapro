#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <string>
#include <cstring>

using namespace luabridge;

typedef struct _Lua
{
    lua_State* L;
}Lua;

static int g_init = 1;

Lua* proCreate();

void proClose(Lua* lua);

//加载lua文件
bool proLoadLuaFile(Lua* lua, const char* file);

//导出C函数
#define expCFunction(name, func, lvm) \
    getGlobalNamespace(lvm->L).addFunction(name, func)

//注册类1
#define regClass(cls, lvm) \
	getGlobalNamespace(lvm->L).beginClass<cls>(#cls).endClass();

//注册类2
#define expClass(cls, name, ctor, lvm) \
	getGlobalNamespace(lvm->L).beginClass<cls>(name).addConstructor<ctor>().endClass();

#define expClass2(cls, ctor, lvm) \
	expClass(cls, #cls, ctor, lvm)

//导出一般类方法
#define expClassFunction(cls, name, func, func_alias, lvm) \
	getGlobalNamespace(lvm->L).beginClass<cls>(name).addFunction(func_alias, &cls::func).endClass();

#define expClassFunction2(cls, func, lvm) \
	expClassFunction(cls, #cls, func, #func, lvm)

//导出静态类方法
#define expClassSFunction(cls, name, func, func_alias, lvm) \
	getGlobalNamespace(lvm->L).beginClass<cls>(name).addStaticFunction(func_alias, &cls::func).endClass();

#define expClassSFunction2(cls, func, lvm) \
	expClassSFunction(cls, #cls, func, #func, lvm)

//调用lua函数（标准）
template<typename... Args>
LuaRef callLuaStdFunction(const char* name, Lua* lua,  Args...args)
{
	LuaRef func = luabridge::getGlobal(lua->L, name);
	return func(args...);
}

#define expProtobufToLuaFunction(func_name, lvm) \
	lua_pushcfunction(lvm->L, protobuf_to_lua); \
	lua_setglobal(lvm->L, func_name);

#define expLuaToProtobufFunction(func_name, lvm) \
	lua_pushcfunction(lvm->L, lua_to_protobuf); \
	lua_setglobal(lvm->L, func_name);

#define callLuaProtoFunction(name, proto_ptr, lvm) \
	lua_getglobal(lvm->L, name); \
	lua_pushlightuserdata(lvm->L, proto_ptr); \
	lua_pcall(lvm->L, 1, 0, 0);

//#define regComplexFunction()
#define ComplexTest(name, Duck, Message, iit, lvm) \
	lua_getglobal(lvm->L, name); \
	luabridge::push(lvm->L, Duck); \
	luabridge::push(lvm->L, iit); \
	lua_pushlightuserdata(lvm->L, Message); \
	lua_pcall(lvm->L, 3, 0, 0);


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
