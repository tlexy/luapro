#pragma once
#ifndef LUA_HELPER_H
#define LUA_HELPER_H

#include <google/protobuf/message.h>
#include <lua.hpp>

int protobuf_to_lua(lua_State* L);
void protobuf_new_table(lua_State* L, const google::protobuf::Message* msg);
void printTable(lua_State* pState, int index);
void stackDump(lua_State *l);
int lua_to_protobuf(lua_State* L);
void fill_lua_table(lua_State* L, int idx, google::protobuf::Message* msg);
google::protobuf::Message* create_message(const char* type_name);
void traverse_message(const google::protobuf::Message* msg);
#endif

