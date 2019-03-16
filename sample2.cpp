// sample2.cpp : C++和lua的变量交互
//

extern "C"
{
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
};

#include "lua_tinker.h"

static int cpp_int = 100;

int main()
{
    // open lua
    lua_State* L = lua_open();
    // open lua base library
    luaopen_base(L);

    // LuaTinker绑定c++中的变量
    lua_tinker::set(L, "cpp_int", cpp_int);

    // sample1.lua
    lua_tinker::dofile(L, "sample2.lua");

    // 获取sample1.lua中的变量
    int lua_int = lua_tinker::get<int>(L, "lua_int");
    printf("lua_int = %d\n", lua_int);

    lua_close(L);

    return 0;
}
