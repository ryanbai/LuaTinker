-- ��ӡ��Ա����
print(g_test._test)

-- const char* test::is_test()
print(g_test:is_test())

-- test::ret_int()
print(g_test:ret_int())

-- temp
temp = test(4)
print(temp._test)
--��ȡȫ�ֱ�����_testֵ��������A����
a = g_test:get()
-- ���µ�temp������
temp:set(a)

-- test::set(A a), tempӦ����g_testһ��
print(temp._test)

-- �Ƿ��ǻ���
print(temp:is_base())

-- �Ƿ���test��
print(temp:is_test())

-- ��ӡ������Ϣ
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

-- C++ ȫ�ֱ���
print("objinfo(g_test)")
objinfo(g_test)

-- ��lua�е����˹��캯�������Ķ���
print("objinfo(temp)")
objinfo(temp)

-- Aδ���󶨣���ȡ����������Ϣ
print("objinfo(a)")
objinfo(a)

