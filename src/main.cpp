#include <iostream>
#include <string>
#include "luaprop.h"
#include "../test/ctest.h"

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
	expVal("age", 222, lvm);
	expVal("name", "protobuf", lvm);
    expVal("FIRE", think, lvm);

	regClass(AnimalDuck, lvm);

	/*getGlobalNamespace(lvm->L)
		.beginClass<AnimalDuck>("Duck")
		.addConstructor<void(*)(int)>()
		.addStaticFunction("abaname", &AnimalDuck::name)
		.addFunction("aprint", &AnimalDuck::print)
		.endClass();*/

	//getGlobalNamespace(lvm->L)
	//	.beginClass <A>("A")
	//	.addConstructor<void(*)()>()
	//	.addStaticFunction("staticFunc", &A::staticFunc)
	//	.addFunction("func1", &A::func1)
	//	.addFunction("virtualFunc", &A::virtualFunc)
	//	.addFunction("__tostring", &A::toString)     // Metamethod
	//	.endClass();
	//regClass(AnimalDuck);
	expClass(AnimalDuck, "Duck", void(*)(int), lvm);
	expClassFunction(AnimalDuck, "Duck", print, "aprint", lvm);
	expClassSFunction(AnimalDuck, "Duck", name, "abaname", lvm);

    expCFunction("zname", sname, lvm);
	expCFunction("add", add, lvm);
    expCFunction("duck", duck, lvm);

	std::cout << PR(linxing) << std::endl;


	//最后一次保存文件
    proLoadLuaFile(lvm, "base_test.lua");
    std::cin.get();

    proClose(lvm);
    return 0;
}