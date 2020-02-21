-- print(age, name, FIRE)
-- print(add(1222, 232343))
-- duck()
-- print(zname())

-- -- local a = A()
-- -- a:staticFunc()

-- local d = Duck(999)
-- d:print()
-- Duck.abaname()

-- local ttt = "How to fix it?"

-- function msg_1314(a, b, c, du)
--     print(a, b, c)
--     du:print();
--     return a + 19;
-- end

function msg_par(proto)
	local msg = ParseProto(proto)
	print("terst")
	print(222, msg.player_id, msg.player_name)
end

function msg_complex1(d, iit, proto)
	local msg = ParseProto(proto)
	print(999, msg.player_id, msg.player_name)
	print(iit)
	d:print()
end