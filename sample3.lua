-- 打印成员变量
print(g_test._test)

-- const char* test::is_test()
print(g_test:is_test())

-- test::ret_int()
print(g_test:ret_int())

-- temp
temp = test(4)
print(temp._test)
--获取全局变量的_test值，并付给A对象
a = g_test:get()
-- 更新到temp对象中
temp:set(a)

-- test::set(A a), temp应当与g_test一致
print(temp._test)

-- 是否是基类
print(temp:is_base())

-- 是否是test类
print(temp:is_test())

-- 打印对象信息
-------------------------------------------------------------------------------
function objinfo(obj)
    local meta = getmetatable(obj)
    if meta ~= nil then
        metainfo(meta)
    else
        print("no object infomation !!")
    end
end

function metainfo(meta)
    if meta ~= nil then
        local name = meta["__name"]
        if name ~= nil then
            metainfo(meta["__parent"])
            print("<"..name..">")
            for key,value in pairs(meta) do 
                if not string.find(key, "__..") then 
                    if type(value) == "function" then
                        print("\t[f] "..name..":"..key.."()") 
                    elseif type(value) == "userdata" then
                        print("\t[v] "..name..":"..key)
                    end
                end
            end
        end
    end
end
-------------------------------------------------------------------------------

print("g_test    -> ", g_test)
print("temp    -> ", temp)
print("a    -> ", a)

-- C++ 全局变量
print("objinfo(g_test)")
objinfo(g_test)

-- 在lua中调用了构造函数创建的对象
print("objinfo(temp)")
objinfo(temp)

-- A未被绑定，获取不到对象信息
print("objinfo(a)")
objinfo(a)

