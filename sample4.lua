-- ��ӡhaha
print(haha)

-- ��ӡhaha.value
print(haha.value)

-- ��table
print(haha.inside)

-- ��table��valueֵ
print(haha.inside.value)

-- lua����haha.test
haha.test = "input from lua"

-- ��ӡ���
function print_table(arg)
	print("arg = ", arg)
	print("arg.name = ", arg.name)
end

-- ����һ���ֲ�table
function return_table(arg)
	local ret = {}
	ret.name = arg
	return ret
end
