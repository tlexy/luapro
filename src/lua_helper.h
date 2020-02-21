#pragma once
#ifndef LUA_HELPER_H
#define LUA_HELPER_H

#include <google/protobuf/message.h>
#include <lua.hpp>

int protobuf_to_lua(lua_State* L);
void protobuf_new_table(lua_State* L, const google::protobuf::Message* msg);
void printTable(lua_State* pState, int index);
void stackDump(lua_State *l);

class lua_helper
{
public:
	lua_helper();
	~lua_helper();

	static google::protobuf::Message* create_message(const char* type_name);
	static void traverse_message(const google::protobuf::Message* msg);
	static int lua_to_protobuf(lua_State* L);
	static void reg_proto(int proto, const std::string& name);

private:
	static std::map<int, std::string> proto_map;
private:
	static void fill_lua_table(lua_State* L, int idx, google::protobuf::Message* msg);
};

#endif

