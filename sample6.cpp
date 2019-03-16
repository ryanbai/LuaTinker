// sample3.cpp : LuaTinker
//

extern "C"
{
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
};

#include "lua_tinker.h"

int TestFunc(lua_State* L)
{
    printf("# TestFunc exec\n");
    return lua_yield(L, 0);
}

int TestFunc2(lua_State* L, float a)
{
    printf("# TestFunc2(L,%f) exec\n", a);
    return lua_yield(L, 0);
}

class TestClass
{
public:

    int TestFunc(lua_State* L)
    {
        printf("# TestClass::TestFunc exec\n");
        return lua_yield(L, 0);
    }

    int TestFunc2(lua_State* L, float a)
    {
        printf("# TestClass::TestFunc2(L,%f) exec\n", a);
        return lua_yield(L, 0);
    }
};

int main()
{
    lua_State* L = lua_open();
    luaopen_base(L);
    luaopen_string(L);

    // ��TestFunc��TestFunc2��lua
    lua_tinker::def(L, "TestFunc", &TestFunc);
    lua_tinker::def(L, "TestFunc2", &TestFunc2);

    // ��TestClass��
    lua_tinker::class_add<TestClass>(L, "TestClass");
    // ��TestClass��ĳ�Ա����
    lua_tinker::class_def<TestClass>(L, "TestFunc", &TestClass::TestFunc);
    lua_tinker::class_def<TestClass>(L, "TestFunc2", &TestClass::TestFunc2);

    // ��TestClass����
    TestClass g_test;
    lua_tinker::set(L, "g_test", &g_test);

    // sample3.lua
    lua_tinker::dofile(L, "sample6.lua");

    // ����luaЭ��
    lua_newthread(L);
    lua_pushstring(L, "ThreadTest");
    lua_gettable(L, LUA_GLOBALSINDEX);

    // ����Э��
    printf("* lua_resume() exec\n");
    lua_resume(L, 0);

    // TestFunc����
    printf("* lua_resume() exec\n");
    lua_resume(L, 0);

    // TestFunc2 yield
    printf("* lua_resume() exec\n");
    lua_resume(L, 0);

    // TestClass::TestFunc yield
    printf("* lua_resume() exec\n");
    lua_resume(L, 0);

    // TestClass::TestFunc2 yield
    printf("* lua_resume() exec\n");
    lua_resume(L, 0);

    lua_close(L);

    return 0;
}

