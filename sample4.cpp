// sample4.cpp : table 접근 법을 알아본다.
// table 은 빠른 접근을 위해서 stack 에 table을 복사해서 쓰게 된다.
// Lua 에서 테이블은 스택 위에서는 포인터만 올라가고 실제 객체는 내부에서 관리된다.
// 따라서 잘못 사용할 경우 리턴을 깨먹거나 Lua 스택위의 지역 변수들에 문제를 일으킬 수도 있다.

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
    // 눼쉔table
    lua_tinker::table haha(L, "haha");
    // haha.value = 1
    haha.set("value", 1);
    // 눼쉔寧몸綾table
    haha.set("inside", lua_tinker::table(L));
    lua_tinker::table inside = haha.get<lua_tinker::table>("inside");
    // inside.value = 2
    inside.set("value", 2);

    // sample4.lua
    lua_tinker::dofile(L, "sample4.lua");

    // lua櫓�阮천훦est냥逃
    const char* test = haha.get<const char*>("test");
    printf("haha.test = %s\n", test);

    // 댔丹줄珂눼쉔돨table
    lua_tinker::table temp(L);
    temp.set("name", "local table !!");
    lua_tinker::call<void>(L, "print_table", temp);

    // 딧痰lua櫓return_table변鑒럿쀼寧몸애꼬table
    lua_tinker::table ret = lua_tinker::call<lua_tinker::table>(L, "return_table", "give me a table !!");
    printf("ret.name =\t%s\n", ret.get<const char*>("name"));

    lua_close(L);

    return 0;
}

