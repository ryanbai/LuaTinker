-- �������
function test_error()
    print("test_error() called !!")
    test_error_1()
end

-- �ڶ���Ƕ�׵���
function test_error_1()
    print("test_error_1() called !!")
    test_error_2()
end

-- ������Ƕ�׵���
function test_error_2()
    print("test_error_2() called !!")
    -- ������
    test_error_3()
end
