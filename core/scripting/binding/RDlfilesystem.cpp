// Copyright 2016 KeNan Liu
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "RDFileSystem.h"
#include "RDLua.h"
#include "RDLog.h"

// getData(where, filename, mode)
static int lgetData(lua_State *L)
{
    // check args
#if defined(RD_DEBUG)
    int argc = lua_gettop(L);
    if (argc != 3) {
        return luaL_error(L, "[Lua Error]: %s getData() fail, has wrong number of arguments",
                          RD_LIB_FILESYSTEM);
    }
#endif
    
    int type = (int)lua_tointeger(L, 1);
    const char *name = lua_tostring(L, 2);
    int mode = lua_toboolean(L, 3);
    RDData *data = RDFileSystem::getInstance()->getData((RD_FileDir)type, name, mode);
    if (!data) {
        return 0;
    }
    
    // lua_pushlstring can deal with binary string
    lua_pushlstring(L, (const char *)data->getBuffer(), data->getSize());
    
    delete data;
    return 1;
}

static int lputData(lua_State *L)
{
    // check args
#if defined(RD_DEBUG)
    int argc = lua_gettop(L);
    if (argc != 2) {
        return luaL_error(L, "[Lua Error]: %s putData() fail, has wrong number of arguments",
                          RD_LIB_FILESYSTEM);
    }
#endif
    
    const char *path = lua_tostring(L, 1);
    const char *buffer = lua_tostring(L, 2);
    size_t size = lua_rawlen(L, 2);
    bool rtn = RDFileSystem::getInstance()->putData(path, buffer, size);
    
    lua_pushboolean(L, rtn);
    return 1;
}

static const struct luaL_Reg luafs_funcs [] = {
    {"getData", lgetData},
    {"putData", lputData},
    {NULL, NULL}
};

// extern func for register library
LUAMOD_API int luaopen_FileSystem(lua_State *L)
{
    luaL_newlib(L, luafs_funcs);
    return 1;
}
