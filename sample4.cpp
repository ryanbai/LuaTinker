// sample4.cpp : table 立辟 过阑 舅酒夯促.
// table 篮 狐弗 立辟阑 困秦辑 stack 俊 table阑 汗荤秦辑 静霸 等促.
// Lua 俊辑 抛捞喉篮 胶琶 困俊辑绰 器牢磐父 棵扼啊绊 角力 按眉绰 郴何俊辑 包府等促.
// 蝶扼辑 肋给 荤侩且 版快 府畔阑 柄冈芭唱 Lua 胶琶困狼 瘤开 函荐甸俊 巩力甫 老栏懦 荐档 乐促.

extern "C"
{
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
};

#include "lua_tinker.h"

int main()
{
    lua_State* L = lua_open();
    luaopen_base(L);
    // 创建table
    lua_tinker::table haha(L, "haha");
    // haha.value = 1
    haha.set("value", 1);
    // 创建一个子table
    haha.set("inside", lua_tinker::table(L));
    lua_tinker::table inside = haha.get<lua_tinker::table>("inside");
    // inside.value = 2
    inside.set("value", 2);

    // sample4.lua
    lua_tinker::dofile(L, "sample4.lua");

    // lua中设置的test成员
    const char* test = haha.get<const char*>("test");
    printf("haha.test = %s\n", test);

    // 打印临时创建的table
    lua_tinker::table temp(L);
    temp.set("name", "local table !!");
    lua_tinker::call<void>(L, "print_table", temp);

    // 调用lua中return_table函数返回一个局部table
    lua_tinker::table ret = lua_tinker::call<lua_tinker::table>(L, "return_table", "give me a table !!");
    printf("ret.name =\t%s\n", ret.get<const char*>("name"));

    lua_close(L);

    return 0;
}

