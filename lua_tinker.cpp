// lua_tinker.cpp
//
// LuaTinker - Simple and light C++ wrapper for Lua.
//
// Copyright (c) 2005-2007 Kwon-il Lee (zupet@hitel.net)
//
// please check Licence.txt file for licence and legal issues.
//
// modified by ryanbai
// 2017-11-7

#include <iostream>

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
};

#include "lua_tinker.h"

/*---------------------------------------------------------------------------*/
/* init                                                                      */
/*---------------------------------------------------------------------------*/
void lua_tinker::init(lua_State *L)
{
}

/*---------------------------------------------------------------------------*/
/* excution                                                                  */
/*---------------------------------------------------------------------------*/
int lua_tinker::dofile(lua_State *L, const char *filename)
{
    lua_pushcclosure(L, on_error, 0);
    int errfunc = lua_gettop(L);
    int ret     = luaL_loadfile(L, filename);

    if (ret == 0)
    {
        ret = lua_pcall(L, 0, 1, errfunc);
    }
    else
    {
        print_error(L, "%s", lua_tostring(L, -1));
    }

    lua_remove(L, errfunc);
    lua_pop(L, 1);
    return ret;
}

/*---------------------------------------------------------------------------*/
int lua_tinker::dostring(lua_State *L, const char *buff) { return lua_tinker::dobuffer(L, buff, strlen(buff)); }

/*---------------------------------------------------------------------------*/
int lua_tinker::dobuffer(lua_State *L, const char *buff, size_t len)
{
    lua_pushcclosure(L, on_error, 0);
    int errfunc = lua_gettop(L);
    int ret     = luaL_loadbuffer(L, buff, len, "lua_tinker::dobuffer()");

    if (ret == 0)
    {
        ret = lua_pcall(L, 0, 1, errfunc);
    }
    else
    {
        print_error(L, "%s", lua_tostring(L, -1));
    }

    lua_remove(L, errfunc);
    lua_pop(L, 1);
    return ret;
}

/*---------------------------------------------------------------------------*/
/* debug helpers                                                             */
/*---------------------------------------------------------------------------*/
static void call_stack(lua_State* L, int n)
{
    lua_Debug ar;
    while (lua_getstack(L, n, &ar) == 1)
    {
        lua_getinfo(L, "nSlu", &ar);

        const char* indent;
        if(n == 0)
        {
            indent = "->\t";
            lua_tinker::print_error(L, "\t<call stack>");
        }
        else
        {
            indent = "\t";
        }

        if(ar.name)
        {
            lua_tinker::print_error(L, "%s%s() : line %d [%s : line %d]", indent, ar.name, ar.currentline, ar.source, ar.linedefined);
        }
        else
        {
            lua_tinker::print_error(L, "%sunknown : line %d [%s : line %d]", indent, ar.currentline, ar.source, ar.linedefined);
        }

        ++n;
    }
}

/*---------------------------------------------------------------------------*/
int lua_tinker::on_error(lua_State *L)
{
    print_error(L, "%s", lua_tostring(L, -1));

    call_stack(L, 0);

    return 1;
}

/*---------------------------------------------------------------------------*/
void lua_tinker::print_error(lua_State *L, const char* fmt, ...)
{
    char text[4096];

    va_list args;
    va_start(args, fmt);
    vsprintf(text, fmt, args);
    va_end(args);

    lua_pushstring(L, "_ALERT");
    lua_gettable(L, LUA_GLOBALSINDEX);
    if(lua_isfunction(L, -1))
    {
        lua_pushstring(L, text);
        lua_call(L, 1, 0);
    }
    else
    {
        printf("%s\n", text);
        lua_pop(L, 1);
    }
}

/*---------------------------------------------------------------------------*/
void lua_tinker::enum_stack(lua_State *L)
{
    int top = lua_gettop(L);
    print_error(L, "Type:%d", top);
    for (int i = top; i >= 1; --i)
    {
        switch(lua_type(L, i))
        {
        case LUA_TNIL:
            print_error(L, "\t%s", lua_typename(L, lua_type(L, i)));
            break;
        case LUA_TBOOLEAN:
            print_error(L, "\t%s    %s", lua_typename(L, lua_type(L, i)), lua_toboolean(L, i)?"true":"false");
            break;
        case LUA_TLIGHTUSERDATA:
            print_error(L, "\t%s    0x%08p", lua_typename(L, lua_type(L, i)), lua_topointer(L, i));
            break;
        case LUA_TNUMBER:
            print_error(L, "\t%s    %f", lua_typename(L, lua_type(L, i)), lua_tonumber(L, i));
            break;
        case LUA_TSTRING:
            print_error(L, "\t%s    %s", lua_typename(L, lua_type(L, i)), lua_tostring(L, i));
            break;
        case LUA_TTABLE:
            print_error(L, "\t%s    0x%08p", lua_typename(L, lua_type(L, i)), lua_topointer(L, i));
            break;
        case LUA_TFUNCTION:
            print_error(L, "\t%s()    0x%08p", lua_typename(L, lua_type(L, i)), lua_topointer(L, i));
            break;
        case LUA_TUSERDATA:
            print_error(L, "\t%s    0x%08p", lua_typename(L, lua_type(L, i)), lua_topointer(L, i));
            break;
        case LUA_TTHREAD:
            print_error(L, "\t%s", lua_typename(L, lua_type(L, i)));
            break;
        }
    }
}

/*---------------------------------------------------------------------------*/
/* read                                                                      */
/*---------------------------------------------------------------------------*/
template<>
char* lua_tinker::read(lua_State *L, int index)
{
    return (char*)lua_tostring(L, index);
}

template<>
const char* lua_tinker::read(lua_State *L, int index)
{
    return (const char*)lua_tostring(L, index);
}

template<>
char lua_tinker::read(lua_State *L, int index)
{
    return (char)lua_tonumber(L, index);
}

template <>
uint8_t lua_tinker::read(lua_State *L, int index)
{
    return (uint8_t)lua_tonumber(L, index);
}

template <>
int16_t lua_tinker::read(lua_State *L, int index)
{
    return (int16_t)lua_tonumber(L, index);
}

template <>
uint16_t lua_tinker::read(lua_State *L, int index)
{
    return (uint16_t)lua_tonumber(L, index);
}

// int64_t is different between i386 and x86_64 architecture
#if defined(__x86_64__) || defined(__x86_64) || defined(__amd_64) || defined(__amd_64__)
template <>
int64_t lua_tinker::read(lua_State *L, int index)
{
    // 接受字符串和数值
    // 字符串场景: C传入LUA，回传到C， 比如C的玩家UIN，在操作玩家数据时回传UIN
    // 数值: Lua传入C，比如定义在Lua侧的数值，传入需要参数为64bit的C函数
    switch (lua_type(L, index))
    {
        case LUA_TSTRING :
        {
            size_t len = 0;
            const char * number = lua_tolstring(L, index, &len);
            if (len !=8 )
            {
                return luaL_error(L, "Need an 8 length string for int64");
            }
            return *(const int64_t *) number;
        }
        case LUA_TNUMBER:
        {
            return (int64_t)lua_tonumber(L, index);
        }
        default:
            return luaL_error(L, "Need an LUA_TSTRING or LUA_TNUMBER");
    }
}

template <>
uint64_t lua_tinker::read(lua_State *L, int index)
{
    return read<int64_t>(L, index);
}
#else  // __i386__ //32bit
template <>
int64_t lua_tinker::read(lua_State *L, int index)
{
    return (int64_t)lua_tonumber(L, index);
}

template <>
uint64_t lua_tinker::read(lua_State *L, int index)
{
    return (uint64_t)lua_tonumber(L, index);
}
#endif  // architecture

template <>
int lua_tinker::read(lua_State *L, int index)
{
    return (int)lua_tonumber(L, index);
}

template <>
uint32_t lua_tinker::read(lua_State *L, int index)
{
    return (uint32_t)lua_tonumber(L, index);
}

template <>
float lua_tinker::read(lua_State *L, int index)
{
    return (float)lua_tonumber(L, index);
}

template <>
double lua_tinker::read(lua_State *L, int index)
{
    return (double)lua_tonumber(L, index);
}

template <>
bool lua_tinker::read(lua_State *L, int index)
{
    if (lua_isboolean(L, index))
    {
        return lua_toboolean(L, index) != 0;
    }
    else
    {
        return (int)lua_tonumber(L, index) != 0;
    }
}

template<>
void lua_tinker::read(lua_State *L, int index)
{
    return;
}

template <>
lua_tinker::table lua_tinker::read(lua_State *L, int index)
{
    return table(L, index);
}

template <>
lua_tinker::CharsWithLen lua_tinker::read(lua_State *L, int index)
{
    lua_tinker::CharsWithLen temp(NULL, 0);
    size_t len;
    temp.buffer = lua_tolstring(L, index, &len);
    temp.len    = len;
    return temp;
}
/*---------------------------------------------------------------------------*/
/* push                                                                      */
/*---------------------------------------------------------------------------*/
template <>
void lua_tinker::push(lua_State *L, char ret)
{
    lua_pushnumber(L, ret);
}

template <>
void lua_tinker::push(lua_State *L, uint8_t ret)
{
    lua_pushnumber(L, ret);
}
template <>
void lua_tinker::push(lua_State *L, const uint8_t &ret)
{
    lua_pushnumber(L, ret);
}

template <>
void lua_tinker::push(lua_State *L, int16_t ret)
{
    lua_pushnumber(L, ret);
}
template <>
void lua_tinker::push(lua_State *L, const int16_t &ret)
{
    lua_pushnumber(L, ret);
}

template <>
void lua_tinker::push(lua_State *L, uint16_t ret)
{
    lua_pushnumber(L, ret);
}
template <>
void lua_tinker::push(lua_State *L, const uint16_t &ret)
{
    lua_pushnumber(L, ret);
}

#if defined(__x86_64__) || defined(__x86_64) || defined(__amd_64) || defined(__amd_64__)
template <>
void lua_tinker::push(lua_State *L, int64_t ret)
{
    lua_pushlstring(L, (const char *)&ret, sizeof(ret));
}

template <>
void lua_tinker::push(lua_State *L, uint64_t ret)
{
    lua_pushlstring(L, (const char *)&ret, sizeof(ret));
}
#else  // __i386__
template <>
void lua_tinker::push(lua_State *L, int64_t ret)
{
    lua_pushnumber(L, ret);
}

template <>
void lua_tinker::push(lua_State *L, uint64_t ret)
{
    lua_pushnumber(L, ret);
}
#endif  // architecture

template <>
void lua_tinker::push(lua_State *L, int ret)
{
    lua_pushnumber(L, ret);
}
template <>
void lua_tinker::push(lua_State *L, const int &ret)
{
    lua_pushnumber(L, ret);
}

template <>
void lua_tinker::push(lua_State *L, uint32_t ret)
{
    lua_pushnumber(L, ret);
}
template <>
void lua_tinker::push(lua_State *L, const uint32_t &ret)
{
    lua_pushnumber(L, ret);
}

template<>
void lua_tinker::push(lua_State *L, float ret)
{
    lua_pushnumber(L, ret);
}

template<>
void lua_tinker::push(lua_State *L, double ret)
{
    lua_pushnumber(L, ret);
}

template<>
void lua_tinker::push(lua_State *L, char* ret)
{
    lua_pushstring(L, ret);
}

template<>
void lua_tinker::push(lua_State *L, const char* ret)
{
    lua_pushstring(L, ret);
}

template<>
void lua_tinker::push(lua_State *L, bool ret)
{
    lua_pushboolean(L, ret);
}

template <>
void lua_tinker::push(lua_State *L, lua_value *ret)
{
    if (ret)
    {
        ret->to_lua(L);
    }
    else
    {
        lua_pushnil(L);
    }
}

template <>
void lua_tinker::push(lua_State *L, lua_tinker::table ret)
{
    lua_pushvalue(L, ret.m_obj->m_index);
}

template <>
void lua_tinker::push(lua_State *L, lua_tinker::CharsWithLen ret)
{
    lua_pushlstring(L, ret.buffer, ret.len);
}

template <>
void lua_tinker::push(lua_State *L, const lua_tinker::CharsWithLen &ret)
{
    lua_pushlstring(L, ret.buffer, ret.len);
}

template <>
void lua_tinker::push(lua_State *L, lua_tinker::CharsWithLen &ret)
{
    lua_pushlstring(L, ret.buffer, ret.len);
}

/*---------------------------------------------------------------------------*/
/* pop                                                                       */
/*---------------------------------------------------------------------------*/
template<>
void lua_tinker::pop(lua_State *L)
{
    lua_pop(L, 1);
}

template<>
lua_tinker::table lua_tinker::pop(lua_State *L)
{
    return table(L, lua_gettop(L));
}

/*---------------------------------------------------------------------------*/
/* Tinker Class Helper                                                       */
/*---------------------------------------------------------------------------*/
static void invoke_parent(lua_State *L)
{
    lua_pushstring(L, "__parent");
    lua_rawget(L, -2);
    if(lua_istable(L,-1))
    {
        lua_pushvalue(L,2);
        lua_rawget(L, -2);
        if(!lua_isnil(L,-1))
        {
            lua_remove(L,-2);
        }
        else
        {
            lua_remove(L, -1);
            invoke_parent(L);
            lua_remove(L,-2);
        }
    }
}

/*---------------------------------------------------------------------------*/
int lua_tinker::array_get(lua_State *L)
{
    // 非数字类型表明是类变量，否则当数组处理
    if (!lua_isnumber(L, 2))
    {
        lua_pushstring(L, "array index must be numberic.");
        lua_error(L);
    }

    lua2object<var_base *>::invoke(L, 1)->get(L);
    lua_settop(L, 3);
    return 1;
}

int lua_tinker::array_set(lua_State *L)
{
    if (!lua_isnumber(L, 2))
    {
        lua_pushstring(L, "array index must be numeric.");
        lua_error(L);
    }

    lua2object<var_base *>::invoke(L, 1)->set(L);
    lua_settop(L, 3);
    return 0;
}
int lua_tinker::array_destroy(lua_State *L)
{
    // DEBUG_LOG(0, 0, "array_destroy called %p", ((user *)lua_touserdata(L, 1))->m_p);
    user* u = (user *)lua_touserdata(L, 1);
    u->~user();
    return 0;
}


/*---------------------------------------------------------------------------*/
int lua_tinker::meta_get(lua_State *L)
{
    // 传入表 和 索引参数
    // stack: 1.类(userdata) 2.变量(string)
    lua_getmetatable(L, 1);
    // stack: 1.类(userdata) 2.变量(string) 3.meta(table)
    lua_pushvalue(L, 2);
    // stack: 1.类(userdata) 2.变量(string) 3.meta(table) 4.变量(string)
    lua_rawget(L, -2);
    // stack: 1.类(userdata) 2.变量(string) 3.meta(table) 4.meta[变量]value值(userdata)

    // 如果存在userdata 存在该变量
    if (lua_isuserdata(L, -1))
    {
        user2type<var_base *>::invoke(L, -1)->get(L);
        // stack: 1.类(userdata) 2.变量(string) 3.meta(table) 4.meta[变量]value值(userdata) 5.实际值
        lua_remove(L, -2);
        // stack: 1.类(userdata) 2.变量(string) 3.meta(table) 4.实际值
    }
    else if (lua_isnil(L, -1))
    {
        // stack: 1.类(userdata) 2.变量(string) 3.meta(table) 4.nil
        lua_remove(L, -1);
        // stack: 1.类(userdata) 2.变量(string) 3.meta(table)
        invoke_parent(L);

        // fix bug by fergus
        // 调用父类也需调用get
        if (lua_isuserdata(L, -1))
        {
            // stack: 1.类(userdata) 2.变量(string) 3.meta(table) 4.父类中的变量(userdata)
            user2type<var_base *>::invoke(L, -1)->get(L);
            // stack: 1.类(userdata) 2.变量(string) 3.meta(table) 4.父类中的变量(userdata) 5.实际值
            lua_remove(L, -2);
            // stack: 1.类(userdata) 2.变量(string) 3.meta(table) 4.实际值
        }
        else if (lua_isnil(L, -1))
        {
            // stack: 1.类(userdata) 2.变量(string) 3.meta(table) 4.nil
            lua_pushfstring(L, "can't find '%s' class variable. (forgot registering class variable ?)",
                            lua_tostring(L, 2));
            lua_error(L);
        }
    }

    lua_remove(L, -2);
    // stack: 1.类(userdata) 2.变量(string) 3.实际值
    return 1;
}

/*---------------------------------------------------------------------------*/
int lua_tinker::meta_set(lua_State *L)
{
    // stack: 1.类(userdata) 2.变量(string) 3.要赋的值
    lua_getmetatable(L, 1);
    // stack: 1.类(userdata) 2.变量(string) 3.要赋的值 4.类meta(table)
    lua_pushvalue(L, 2);
    // stack: 1.类(userdata) 2.变量(string) 3.要赋的值 4.类meta(table)
    // 5.变量(string)
    lua_rawget(L, -2);
    // stack: 1.类(userdata) 2.变量(string) 3.要赋的值 4.类meta(table)
    // 5.meta[变量](userdata mem_var指针)

    if (lua_isuserdata(L, -1))
    {
        user2type<var_base *>::invoke(L, -1)->set(L);
        // stack: 1.类(userdata) 2.变量(string) 3.要赋的值 4.类meta(table) 5.meta[变量](userdata mem_var指针)
    }
    else if (lua_isnil(L, -1))
    {
        // stack: 1.类(userdata) 2.变量(string) 3.要赋的值 4.类meta(table) 5.nil
        lua_remove(L, -1);
        // stack: 1.类(userdata) 2.变量(string) 3.要赋的值 4.类meta(table)
        lua_pushvalue(L, 2);
        // stack: 1.类(userdata) 2.变量(string) 3.要赋的值 4.类meta(table) 5.变量(string)
        lua_pushvalue(L, 4);
        // stack: 1.类(userdata) 2.变量(string) 3.要赋的值 4.类meta(table) 5.变量(string) 6.类meta(table)
        invoke_parent(L);

        // stack: 1.类(userdata) 2.变量(string) 3.要赋的值 4.类meta(table) 5.变量(string) 6.类meta(table) 7.获取到父类的变量(userdata mem_var指针)
        if (lua_isuserdata(L, -1))
        {
            user2type<var_base *>::invoke(L, -1)->set(L);
        }
        else if (lua_isnil(L, -1))
        {
            // stack: 1.类(userdata) 2.变量(string) 3.要赋的值 4.类meta(table) 5.变量(string) 6.类meta(table) 7.nil
            lua_pushfstring(L, "can't find '%s' class variable. (forgot registering class variable ?)",
                            lua_tostring(L, 2));
            lua_error(L);
        }
    }

    lua_settop(L, 3);
    // stack: 1.类(userdata) 2.变量(string) 3.要赋的值
    return 0;
}

/*---------------------------------------------------------------------------*/
void lua_tinker::push_meta(lua_State *L, const char *name) { lua_getglobal(L, name); }

/*---------------------------------------------------------------------------*/
/* table object on stack                                                     */
/*---------------------------------------------------------------------------*/
lua_tinker::table_obj::table_obj(lua_State *L, int index) : m_L(L), m_index(index), m_ref(0)
{
    if(lua_isnil(m_L, m_index))
    {
        m_pointer = NULL;
        lua_remove(m_L, m_index);
    }
    else
    {
        m_pointer = lua_topointer(m_L, m_index);
    }
}

lua_tinker::table_obj::~table_obj()
{
    if(validate())
    {
        lua_remove(m_L, m_index);
    }
}

void lua_tinker::table_obj::inc_ref() { ++m_ref; }

void lua_tinker::table_obj::dec_ref()
{
    if (--m_ref == 0)
    {
        delete this;
    }
}

bool lua_tinker::table_obj::validate()
{
    if(m_pointer != NULL)
    {
        if(m_pointer == lua_topointer(m_L, m_index))
        {
            return true;
        }
        else
        {
            int top = lua_gettop(m_L);

            for(int i=1; i<=top; ++i)
            {
                if(m_pointer == lua_topointer(m_L, i))
                {
                    m_index = i;
                    return true;
                }
            }

            m_pointer = NULL;
            return false;
        }
    }
    else
    {
        return false;
    }
}

/*---------------------------------------------------------------------------*/
/* Table Object Holder                                                       */
/*---------------------------------------------------------------------------*/
lua_tinker::table::table(lua_State* L)
{
    lua_newtable(L);

    m_obj = new table_obj(L, lua_gettop(L));

    m_obj->inc_ref();
}

lua_tinker::table::table(lua_State *L, const char *name)
{
    lua_getglobal(L, name);

    if (lua_istable(L, -1) == 0)
    {
        lua_pop(L, 1);
        lua_newtable(L);
        lua_setglobal(L, name);
        lua_getglobal(L, name);
    }

    m_obj = new table_obj(L, lua_gettop(L));
    m_obj->inc_ref();
}

lua_tinker::table::table(lua_State *L, int index)
{
    if (index < 0)
    {
        index = lua_gettop(L) + index + 1;
    }

    m_obj = new table_obj(L, index);
    m_obj->inc_ref();
}

lua_tinker::table::table(const table &input)
{
    m_obj = input.m_obj;
    m_obj->inc_ref();
}

void lua_tinker::table::operator=(const table &input)
{
    // 防止内存泄漏
    if (m_obj)
    {
        m_obj->dec_ref();
    }

    m_obj = input.m_obj;
    m_obj->inc_ref();
}

lua_tinker::table::~table() { m_obj->dec_ref(); }

/*---------------------------------------------------------------------------*/
