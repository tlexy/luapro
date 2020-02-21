#include "lua_helper.h"
//#include "proto/seven_day.pb.h"


void traverse_message(const google::protobuf::Message* msg)
{
	if (!msg)
	{
		return;
	}
	const google::protobuf::Descriptor* des = msg->GetDescriptor();
	const google::protobuf::Reflection* ref = msg->GetReflection();

	std::string name = des->name();

	for (int i = 0; i < des->field_count(); ++i)
	{
		const google::protobuf::FieldDescriptor* fd = des->field(i);
		std::cout << "field: " << fd->name() << std::endl;
		if (fd->is_repeated())
		{
			for (int walk = 0; walk < ref->FieldSize(*msg, fd); ++walk)
			{
				switch (fd->cpp_type())
				{
				case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT32:
					std::cout << "\element: " << ref->GetRepeatedInt32(*msg, fd, walk) << std::endl;
					break;
				case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT64:
					std::cout << "\element: " << ref->GetRepeatedInt64(*msg, fd, walk) << std::endl;
					break;
				case google::protobuf::FieldDescriptor::CppType::CPPTYPE_STRING:
					std::cout << "\element: " << ref->GetRepeatedString(*msg, fd, walk) << std::endl;
					break;
				case google::protobuf::FieldDescriptor::CppType::CPPTYPE_MESSAGE:
					{
						//traverse_message(&ref->GetMessage(*msg, fd));
						const google::protobuf::Message& sub_msg = ref->GetRepeatedMessage(*msg, fd, walk);
						traverse_message(&sub_msg);
					}
					break;
				default:
					break;
				}
			}
			continue;
		}
		switch (fd->cpp_type())
		{
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT32:
			std::cout << "\tvalue: " << ref->GetInt32(*msg, fd) << std::endl;
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT64:
			std::cout << "\tvalue: " << ref->GetInt64(*msg, fd) << std::endl;
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_MESSAGE:
			traverse_message(&ref->GetMessage(*msg, fd));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_STRING:
			std::cout << "\tvalue: " << ref->GetString(*msg, fd) << std::endl;
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_DOUBLE:
			std::cout << "\tvalue: " << ref->GetDouble(*msg, fd) << std::endl;
		default:
			break;
		}
	}
}

google::protobuf::Message* create_message(const char* type_name)
{
	google::protobuf::Message* message = NULL;

	const google::protobuf::Descriptor* des = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
	if (des)
	{
		const google::protobuf::Message* prototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(des);
		if (prototype)
		{
			message = prototype->New();
		}
	}
	return message;
}

void fill_lua_table(lua_State* L, int idx, google::protobuf::Message* msg)
{
	/// 作副本压栈。
	lua_pushvalue(L, idx);

	const google::protobuf::Descriptor* des = msg->GetDescriptor();
	const google::protobuf::Reflection* ref = msg->GetReflection();

	int tt = LUA_TNIL;
	for (int i = 0; i < des->field_count(); ++i)
	{
		const google::protobuf::FieldDescriptor* fd = des->field(i);
		std::string fd_name = fd->name();
		lua_getfield(L, -1, fd->name().c_str());
		if (fd->is_repeated())
		{
			google::protobuf::FieldDescriptor::CppType c_type = fd->cpp_type();
			int arr_len = lua_rawlen(L, -1);
			for (int arr_i = 1; arr_i < arr_len; ++arr_i)
			{
				lua_rawgeti(L, -1, arr_i);
				switch (c_type)
				{
				case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT32:
					ref->AddInt32(msg, fd, lua_tointeger(L, -1));
					break;
				case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT64:
					ref->AddInt64(msg, fd, lua_tointeger(L, -1));
					break;
				case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT32:
					ref->AddUInt32(msg, fd, lua_tointeger(L, -1));
					break;
				case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT64:
					ref->AddUInt64(msg, fd, lua_tointeger(L, -1));
					break;
				case google::protobuf::FieldDescriptor::CppType::CPPTYPE_DOUBLE:
					ref->AddDouble(msg, fd, lua_tonumber(L, -1));
					break;
				case google::protobuf::FieldDescriptor::CppType::CPPTYPE_FLOAT:
					ref->AddFloat(msg, fd, lua_tonumber(L, -1));
					break;
				case google::protobuf::FieldDescriptor::CppType::CPPTYPE_BOOL:
					ref->AddBool(msg, fd, lua_toboolean(L, -1));
					break;
				case google::protobuf::FieldDescriptor::CppType::CPPTYPE_STRING:
					ref->AddString(msg, fd, lua_tostring(L, -1));
					break;
				case google::protobuf::FieldDescriptor::CppType::CPPTYPE_MESSAGE:
				{
					google::protobuf::Message* sub_msg = ref->AddMessage(msg, fd); //lua_helper::create_message(fd->name().c_str());
																				   //AddAllocatedMessage
					fill_lua_table(L, -1, sub_msg);
				}
				break;
				default:
					break;
				}
				lua_pop(L, 1);
			}
			lua_pop(L, 1);
			continue;
		}
		//
		switch (fd->cpp_type())
		{
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT32:
			ref->SetInt32(msg, fd, lua_tointeger(L, -1));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT64:
			ref->SetInt64(msg, fd, lua_tointeger(L, -1));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT32:
			ref->AddUInt32(msg, fd, lua_tointeger(L, -1));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT64:
			ref->AddUInt64(msg, fd, lua_tointeger(L, -1));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_DOUBLE:
			ref->SetDouble(msg, fd, lua_tonumber(L, -1));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_FLOAT:
			ref->SetFloat(msg, fd, lua_tonumber(L, -1));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_BOOL:
			ref->SetBool(msg, fd, lua_toboolean(L, -1));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_STRING:
			ref->SetString(msg, fd, lua_tostring(L, -1));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_MESSAGE:
		{
			google::protobuf::Message* sub_msg = ref->MutableMessage(msg, fd);
			fill_lua_table(L, -1, sub_msg);
		}
		break;
		default:
			break;
		}
		lua_pop(L, 1);
	}
	lua_pop(L, 1);
}

int lua_to_protobuf(lua_State* L)
{
	const char* name = luaL_checkstring(L, 1);
	int type = lua_type(L, 2);
	if (LUA_TTABLE != type)
	{
		//这里要报错
		return 0;
	}
	
	google::protobuf::Message* msg = create_message(name);
	if (msg == NULL)
	{
		//找不到协议定义
		return 0;
	}
	fill_lua_table(L, 2, msg);
	traverse_message(msg);
	return 1;
}

void protobuf_new_table(lua_State* L, const google::protobuf::Message* msg)
{
	const google::protobuf::Descriptor* des = msg->GetDescriptor();
	const google::protobuf::Reflection* ref = msg->GetReflection();

	lua_createtable(L, 0, des->field_count());
	for (int i = 0; i < des->field_count(); ++i)
	{
		const google::protobuf::FieldDescriptor* fd = des->field(i);
		const std::string& key = fd->name();
		lua_pushstring(L, key.c_str());//key
		if (fd->is_repeated())
		{
			int arr_len = ref->FieldSize(*msg, fd);
			lua_createtable(L, arr_len, 0);
			for (int j = 0; j < arr_len; ++j)
			{
				lua_pushinteger(L, j + 1);
				switch (fd->cpp_type())
				{
				case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT32:
					lua_pushinteger(L, ref->GetRepeatedInt32(*msg, fd, j));
					break;
				case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT64:
					lua_pushinteger(L, ref->GetRepeatedInt64(*msg, fd, j));
					break;
				case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT32:
					lua_pushinteger(L, ref->GetRepeatedUInt32(*msg, fd, j));
					break;
				case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT64:
					lua_pushinteger(L, ref->GetRepeatedUInt64(*msg, fd, j));
					break;
				case google::protobuf::FieldDescriptor::CppType::CPPTYPE_DOUBLE:
					lua_pushnumber(L, ref->GetRepeatedDouble(*msg, fd, j));
					break;
				case google::protobuf::FieldDescriptor::CppType::CPPTYPE_FLOAT:
					lua_pushnumber(L, ref->GetRepeatedFloat(*msg, fd, j));
					break;
				case google::protobuf::FieldDescriptor::CppType::CPPTYPE_BOOL:
					lua_pushboolean(L, ref->GetRepeatedBool(*msg, fd, j));
					break;
				case google::protobuf::FieldDescriptor::CppType::CPPTYPE_ENUM:
					lua_pushinteger(L, ref->GetRepeatedEnumValue(*msg, fd, j));
					break;
				case google::protobuf::FieldDescriptor::CppType::CPPTYPE_STRING:
					{
					const std::string& str = ref->GetRepeatedString(*msg, fd, j);
					lua_pushstring(L, str.c_str());
					}
					break;
				case google::protobuf::FieldDescriptor::CppType::CPPTYPE_MESSAGE:
					protobuf_new_table(L, &ref->GetRepeatedMessage(*msg, fd, j));
					break;
				default:
					break;
				}
				lua_rawset(L, -3);
			}
			lua_rawset(L, -3);
			continue;
		}
		//
		switch (fd->cpp_type())
		{
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT32:
			lua_pushinteger(L, ref->GetInt32(*msg, fd));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT64:
			lua_pushinteger(L, ref->GetInt64(*msg, fd));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT32:
			lua_pushinteger(L, ref->GetUInt32(*msg, fd));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT64:
			lua_pushinteger(L, ref->GetUInt64(*msg, fd));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_DOUBLE:
			lua_pushnumber(L, ref->GetDouble(*msg, fd));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_FLOAT:
			lua_pushnumber(L, ref->GetFloat(*msg, fd));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_BOOL:
			lua_pushboolean(L, ref->GetBool(*msg, fd));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_ENUM:
			lua_pushinteger(L, ref->GetEnumValue(*msg, fd));
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_STRING:
			{
			const std::string& str = ref->GetString(*msg, fd);
			lua_pushstring(L, str.c_str());
			}
			break;
		case google::protobuf::FieldDescriptor::CppType::CPPTYPE_MESSAGE:
			protobuf_new_table(L, &ref->GetMessage(*msg, fd));
			break;
		default:
			break;
		}
		lua_settable(L, -3);
	}
}

int protobuf_to_lua(lua_State* L)
{
	int type = lua_type(L, 1);
	void* ud = lua_touserdata(L, 1);

	if (LUA_TUSERDATA != type && LUA_TLIGHTUSERDATA != type)
	{
		return 0;
	}
	google::protobuf::Message* msg = (google::protobuf::Message*)ud;
	protobuf_new_table(L, msg);
	lua_remove(L, 1);
	//stackDump(L);
	return 1;
}

void printTable(lua_State* pState, int index)
{
	int top = lua_gettop(pState);

	/// 把栈上给定索引处的元素作一个副本压栈。
	lua_pushvalue(pState, index);

	fprintf(stdout, "{\n");

	lua_pushnil(pState);
	int len = 0;
	while (lua_next(pState, -2))
	{
		len = lua_gettop(pState);
		fprintf(stdout, "\t");
		int type = lua_type(pState, -2);
		switch (type)
		{
		case LUA_TNUMBER:
			fprintf(stdout, "%g", lua_tonumber(pState, -2));
			break;
		case LUA_TBOOLEAN:
			fprintf(stdout, "%d", int(lua_toboolean(pState, -2)));
			break;
		case LUA_TSTRING:
			fprintf(stdout, "%s", lua_tostring(pState, -2));
			break;
		default:
			fprintf(stdout, "%s:%p", lua_typename(pState, type), lua_topointer(pState, -2));
			break;
		}

		fprintf(stdout, "\t\t=\t");

		type = lua_type(pState, -1);
		switch (type)
		{
		case LUA_TNUMBER:
			fprintf(stdout, "%g", lua_tonumber(pState, -1));
			break;
		case LUA_TBOOLEAN:
			fprintf(stdout, "%d", int(lua_toboolean(pState, -1)));
			break;
		case LUA_TSTRING:
			fprintf(stdout, "%s", lua_tostring(pState, -1));
			break;
		case  LUA_TFUNCTION:
			fprintf(stdout, "%s:%p", lua_typename(pState, type), lua_topointer(pState, -1));
			break;
		case LUA_TUSERDATA:
			fprintf(stdout, "%s:%s", lua_typename(pState, type), lua_tostring(pState, -1));
			break;
		default:
			fprintf(stdout, "%s:%p", lua_typename(pState, type), lua_topointer(pState, -1));
		}

		fprintf(stdout, "\n");

		lua_pop(pState, 1);
	}

	fprintf(stdout, "}\n");

	lua_settop(pState, top);
}

void stackDump(lua_State *l)
{
	int i;
	int top = lua_gettop(l);
	printf("------start-----%d\n", top);

	for (i = 1; i <= top; i++)
	{
		int t = lua_type(l, i);
		printf("type: %s value:", lua_typename(l, t));

		switch (t)
		{
		case LUA_TSTRING:
			printf("%s", lua_tostring(l, i));
			break;
		case LUA_TBOOLEAN:
			printf(lua_toboolean(l, i) ? "true" : "false");
			break;
		case LUA_TNUMBER:
			printf("%g", lua_tonumber(l, i));
			break;
		case LUA_TTABLE:
			printTable(l, i);
			break;
		default:
			printf("%s", lua_typename(l, t));
			break;
		}
		printf("\n");
	}

	printf("------end----\n");
}

