// sample1.cpp : C++ 和 lua函数互调
//

#include <iostream>

extern "C"
{
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
};

#include "lua_tinker.h"

int cpp_func(int arg1, int arg2)
{
    return arg1 + arg2;
}

int main()
{
    // open lua
    lua_State* L = lua_open();
    // open base library
    luaopen_base(L);
    // 将cpp_func绑定到lua
    lua_tinker::def(L, "cpp_func", cpp_func);

    // sample1.lua
    lua_tinker::dofile(L, "sample1.lua");

    // 调用sample1.lua中的lua_func函数
    int result = lua_tinker::call<int>(L, "lua_func", 3, 4);

    // lua_func(3,4)
    printf("lua_func(3,4) = %d\n", result);

    // close lua
    lua_close(L);

    return 0;
}
