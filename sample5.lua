-- 调用入口
function test_error()
    print("test_error() called !!")
    test_error_1()
end

-- 第二层嵌套调用
function test_error_1()
    print("test_error_1() called !!")
    test_error_2()
end

-- 第三层嵌套调用
function test_error_2()
    print("test_error_2() called !!")
    -- 不存在
    test_error_3()
end
