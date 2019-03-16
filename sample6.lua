-- lua coroutine
function ThreadTest()
	print("ThreadTest enter")
	
	print("TestFunc test")
	-- lua_yield()»á¹ÒÆð
	TestFunc()
	TestFunc2(1.2)
	print("TestFunc finish")
	
	print("g_test::TestFunc() test")
	g_test:TestFunc()
	g_test:TestFunc2(2.3)
	print("g_test::TestFunc() finish")
	
	print("ThreadTest finish")
end
