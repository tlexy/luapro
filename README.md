# luapro

## 介绍
* luapro+， 一个C++和lua相互调用的库，支持protobuf到lua table之间相互转换。

## 用法

### 通用
```c++
//新建一个lua虚拟机
Lua lvm = proCreate();

//结束一个lua虚拟机
proClose(lvm);
```
### 导出C函数
```c++
int add(int a, int b)
{
    return a + b;
}
//导出
expCFunction("add", add, lvm);
```

### 导出C变量及常量
```c++
//变量
int var_i = 0;
expVal("var_name", var_i, lvm);

//常量的值
enum elua_err
{
    lua_err_1,
    lua_err_2
}
expVal("cons_name", lua_err_1, Lvm);
```

### 导出C++类及方法
```c++
class AnimalDuck
{
public:
    AnimalDuck()
    {}
    AnimalDuck(int age)
        :_age(age)
    {}

    void print()
    {
        std::cout << "I\' a  duck, my age is " << _age << std::endl;
    }

	static void name()
	{
		std::cout << "static function name..." << std::endl;
	}

private:
    int _age;
};
//单纯地导出一个类(如果不导出这个类的情况下，lua调用C++函数时有用到这个类对象指针时，程序会出错)
regClass(AnimalDuck, lvm);

//导出类的同时，导出其构造函数
expClass(AnimalDuck, "Duck", void(*)(int), lvm);//可以在lua中这样创建一个Duck对象：local d = Duck(888)

//导出类对象方法
expClassFunction(AnimalDuck, "Duck", print, "aprint", lvm);//lua中调用：d:aprint()

//导出类静态方法
expClassFunction(AnimalDuck, "Duck", name, "aname", lvm);//lua中调用：Duck.aname()

```
### 导出类变量及常量

>一个好的设计不应该导出类变量及常量

### C++调用lua函数
```lua
--lua文件
function msg_1314(a, b, c, du)
    print(a, b, c)
    du:print();
    return a + 19;
end
```
```c++
expClassFunction2(AnimalDuck, print, lvm); //注册AnimalDuck类及print方法
AnimalDuck du;//

//通过callLuaStdFunction调用lua函数时，所传递的自定义数据结构（类及结构体）必须要先注册
LuaRef ret = callLuaStdFunction("msg_1314", lvm, 1314, "msg", false, &du, L);
std::cout << "lua func return " << ret.cast<int>() << std::endl;
```

### 将一个protobuf消息类转换为lua中的table
```
syntax = "proto2";

message MsgSevenResp
{
	required int32 player_id = 1;
	required string player_name = 2; 
	repeated int32 records = 3;
    required double money = 4;
}

```
```lua
-- lua文件 test1.lua
print("test1.lua for test")

function msg_1311(message)
    -- 将一个C++的protobuf消息转换为lua中的table
	local RecvMsg = parseProto(message)
	if RecvMsg == nil then 
		print("RecvMsg is nil.")
		return 
	end
	print(RecvMsg.player_id)
	print(RecvMsg.player_name)
	print("records is :")
	for i = 1, #RecvMsg.records, 1 do 
		print(RecvMsg.records[i])
	end
	print("money is:", RecvMsg.money)
end
```
```c++
//C++代码
MsgSevenResp resp;
resp.set_player_id(12354);
resp.set_money(5555.55);
resp.set_player_name("protobuf");
for (int i = 0; i < 10; ++i)
{
	resp.add_records(i);
}

int flag = proLoadLua("test1.lua");
if (flag != 0)
{
    proPrintLuaErr();//打印lua出错信息
    proClose(L)
}

//为protobuf到lua的转换函数起一个名字，这个操作只需要一次即可
expProtobufToLuaFunction("ParseProto", lvm);

//调用lua函数
callLuaProtoFunction("msg_1311", &resp, lvm);

```
### 将一个lua中的table转换为protobuf消息类

### C++调用lua函数（混合参数版本）
>混合参数指的是：C++传递到lua函数中的某些自定义数据结构并没有注册
>上面的例子中，类MsgSevenResp并没有注册，而类AnimalDuck已经注册过了，
>如果lua函数中有MsgSevenResp类对象和AnimalDuck类对象作为参数时，
>这时，为了调用的正确性，不能直接调用callLuaStdFunction来调用lua函数
