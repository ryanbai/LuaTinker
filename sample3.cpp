// sample3.cpp : LuaTinker 狼 努贰胶 扁瓷阑 舅酒夯促.
//

extern "C"
{
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
};

#include "lua_tinker.h"

struct A
{
    A(int v) : value(v) {}
    int value;
};

struct base
{
    base() {}

    const char* is_base(){ return "this is base"; }
};

class test : public base
{
public:
    test(int val) : _test(val) {}
    ~test() {}

    const char* is_test(){ return "this is test"; }

    void ret_void() {}
    int ret_int()               { return _test;           }
    int ret_mul(int m) const    { return _test * m;       }
    A get()                     { return A(_test);        }
    void set(A a)               { _test = a.value;        }

    int _test;
};

test g_test(11);

int main()
{
    lua_State* L = lua_open();
    luaopen_base(L);
    // opens the string lib
    luaopen_string(L);

    // 绑定base类
    lua_tinker::class_add<base>(L, "base");
    // 绑定成员函数base::is_base
    lua_tinker::class_def<base>(L, "is_base", &base::is_base);

    // 绑定test类
    lua_tinker::class_add<test>(L, "test");
    // test继承自base
    lua_tinker::class_inh<test, base>(L);
    // 绑定test构造函数
    lua_tinker::class_con<test>(L, lua_tinker::constructor<test,int>);
    // 绑定test的成员函数和成员变量
    lua_tinker::class_def<test>(L, "is_test", &test::is_test);
    lua_tinker::class_def<test>(L, "ret_void", &test::ret_void);
    lua_tinker::class_def<test>(L, "ret_int", &test::ret_int);
    lua_tinker::class_def<test>(L, "ret_mul", &test::ret_mul);
    lua_tinker::class_def<test>(L, "get", &test::get);
    lua_tinker::class_def<test>(L, "set", &test::set);
    lua_tinker::class_mem<test>(L, "_test", &test::_test);

    // 绑定全局变量
    lua_tinker::set(L, "g_test", &g_test);

    // sample3.lua
    lua_tinker::dofile(L, "sample3.lua");

    lua_close(L);

    return 0;
}

