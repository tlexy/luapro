#include <iostream>
#include <string>
#include "luaprop.h"
#include "../test/ctest.h"
#include "lua_helper.h"
#include "../proto/seven_day.pb.h"

int add(int a, int b)
{
    return a + b;
}

static std::string sname()
{
    return std::string("zzz~");
}

AnimalDuck* duck()
{
	std::cout << "gua gua gua..." << std::endl;
    return new AnimalDuck(444);
}

#define PR(LLL) #LLL

int main()
{
    Lua* lvm = proCreate();
	//expVal("age", 222, lvm);
	//expVal("name", "protobuf", lvm);
 //   expVal("FIRE", think, lvm);

	//regClass(AnimalDuck, lvm);
	////regClass(AnimalDuck);
	//expClass(AnimalDuck, "Duck", void(*)(int), lvm);
	////expClassFunction(AnimalDuck, "Duck", print, "aprint", lvm);
	expClassFunction2(AnimalDuck, print, lvm);
	//expClassSFunction(AnimalDuck, "Duck", name, "abaname", lvm);

	AnimalDuck du;

 //   expCFunction("zname", sname, lvm);
	//expCFunction("add", add, lvm);
 //   expCFunction("duck", duck, lvm);

	//std::cout << PR(linxing) << std::endl;

	//为protobuf到lua的转换函数起一个名字，这个操作只需要一次即可
	expProtobufToLuaFunction("ParseProto", lvm);

	MsgPlayer player;
	player.set_player_id(89898);
	player.set_player_name("protobuf ot test");


	//加载文件
    proLoadLuaFile(lvm, "base_test.lua");

	//LuaRef ret = callLuaStdFunction("msg_1314", lvm, 1414, "msg", false, &du);// func(1314, "msg");
	//std::cout << "lua func return " << ret.cast<int>() << std::endl;

	callLuaProtoFunction("msg_par", &player, lvm);

	player.set_player_name("complex test");
	ComplexTest("msg_complex1", &du, &player, 876, lvm);

    std::cin.get();

    proClose(lvm);
    return 0;
}