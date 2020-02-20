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
addCFunction("add", add, lvm);
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
class Test
{
public:
    int add(int, int);
};
//导出
addFunction("class_func", Test, add, L);

```
### 导出类变量及常量

>一个好的设计不应该导出类变量及常量

### C++调用lua函数
```c++
callLuaFunction("lua_func_name", para1, para2, para3, L);
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
//调用lua函数
callLuaFunction("msg_1311", &resp, L);

```
### 将一个lua中的table转换为protobuf消息类
