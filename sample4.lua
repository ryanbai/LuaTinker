-- 打印haha
print(haha)

-- 打印haha.value
print(haha.value)

-- 子table
print(haha.inside)

-- 子table的value值
print(haha.inside.value)

-- lua增加haha.test
haha.test = "input from lua"

-- 打印表格
function print_table(arg)
	print("arg = ", arg)
	print("arg.name = ", arg.name)
end

-- 返回一个局部table
function return_table(arg)
	local ret = {}
	ret.name = arg
	return ret
end
