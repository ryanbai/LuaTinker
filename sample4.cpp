// sample4.cpp : table ���� ���� �˾ƺ���.
// table �� ���� ������ ���ؼ� stack �� table�� �����ؼ� ���� �ȴ�.
// Lua ���� ���̺��� ���� �������� �����͸� �ö󰡰� ���� ��ü�� ���ο��� �����ȴ�.
// ���� �߸� ����� ��� ������ ���԰ų� Lua �������� ���� �����鿡 ������ ����ų ���� �ִ�.

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
    // ����table
    lua_tinker::table haha(L, "haha");
    // haha.value = 1
    haha.set("value", 1);
    // ����һ����table
    haha.set("inside", lua_tinker::table(L));
    lua_tinker::table inside = haha.get<lua_tinker::table>("inside");
    // inside.value = 2
    inside.set("value", 2);

    // sample4.lua
    lua_tinker::dofile(L, "sample4.lua");

    // lua�����õ�test��Ա
    const char* test = haha.get<const char*>("test");
    printf("haha.test = %s\n", test);

    // ��ӡ��ʱ������table
    lua_tinker::table temp(L);
    temp.set("name", "local table !!");
    lua_tinker::call<void>(L, "print_table", temp);

    // ����lua��return_table��������һ���ֲ�table
    lua_tinker::table ret = lua_tinker::call<lua_tinker::table>(L, "return_table", "give me a table !!");
    printf("ret.name =\t%s\n", ret.get<const char*>("name"));

    lua_close(L);

    return 0;
}

