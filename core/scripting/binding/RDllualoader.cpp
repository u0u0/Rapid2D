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

static int lfromFile(lua_State *L)
{
#if defined(RD_DEBUG)
    // check args
    int argc = lua_gettop(L);
    if (argc != 1) {
        return luaL_error(L, "[Lua Error]: %s fromFile() fail, has wrong number of arguments",
                          RD_LIB_LUALOADER);
    }
#endif
    const char *name = lua_tostring(L, 1);
#if defined(RD_DEBUG)
    if (!name) {
        return luaL_error(L, "[Lua Error]: %s fromFile() fail, arg 1 must be string",
                          RD_LIB_LUALOADER);
    }
#endif
    
    return (LUA_OK == RDLuaLoadFromFile(L, name)) ? 1 : 0;// number of return values
}

static int lfromChunks(lua_State *L)
{
#if defined(RD_DEBUG)
    // check args
    int argc = lua_gettop(L);
    if (argc != 3) {
        return luaL_error(L, "[Lua Error]: %s fromChunks() fail, has wrong number of arguments",
                          RD_LIB_LUALOADER);
    }
#endif
    int type = (int)lua_tointeger(L, 1);
    const char *path = lua_tostring(L, 2);
    const char *key = lua_tostring(L, 3);
#if defined(RD_DEBUG)
    if (!path || !key) {
        return luaL_error(L, "[Lua Error]: %s fromChunks() fail, arg 1,2 must be string",
                          RD_LIB_LUALOADER);
    }
#endif
    
    char *fullPath = RDFileSystem::getInstance()->getFullPath((RD_FileDir)type, path);
    int rtn = RDLuaLoadFromChunks(L, fullPath, key);
    free(fullPath);
    
    if (rtn == LUA_OK) {
        lua_pushboolean(L, 1);
    } else {
        lua_pushboolean(L, 0);
    }
    return 1;// number of return values
}

static const struct luaL_Reg luaLoader_funcs [] = {
    {"fromFile", lfromFile},// load *.lua from RD_FILEDIR_LUA
    {"fromChunks", lfromChunks},// load game.rd
    {NULL, NULL}
};

// extern func for register library
LUAMOD_API int luaopen_LuaLoader(lua_State *L)
{
    luaL_newlib(L, luaLoader_funcs);
    return 1;
}
