# luapro

## 介绍
* luapro+， 一个C++和lua相互调用的库，支持protobuf到lua table之间相互转换。

## 用法

### 通用
```c++
#define L lvm //定义一个宏，方便使用

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
addCFunction("add", add, L);
```

### 导出C变量及常量

### 导出C++类及方法
```c++
class Test
{
public:
    int add(int, int);
};
//导出
addFunction("class_func", Test, add, L);

```
### 导出类变量及常量

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
//lua文件 test1.lua
print("main3.lua for test")

function msg_1311(message)
	local RecvMsg = Parse(message)
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
}
//调用lua函数
callLuaFunction("msg_1311", &resp, L);

```
### 将一个lua中的table转换为protobuf消息类
