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

#include "RDTexture.h"
#include "RDLua.h"
#include "RDLog.h"

static int lnewTexture(lua_State * L)
{
#if defined(RD_DEBUG)
    // chehck args
    int argc = lua_gettop(L);
    if (argc != 1) {
        return luaL_error(L, "[Lua Error]: %s new() fail, has wrong number of arguments", RD_LIB_TEXTURE);
    }
#endif
    const char *name = lua_tostring(L, 1);
#if defined(RD_DEBUG)
    if (!name) {
        return luaL_error(L, "[Lua Error]: %s new() fail, arg 1 must be string", RD_LIB_TEXTURE);
    }
#endif
    
    // create sprite
    RDTexture *t = RDTexture::create(name);
    if (!t) {
        return luaL_error(L, "[Lua Error]: %s new() fail, texture %s not found", RD_LIB_TEXTURE, name);
    }
    
    // create userdata and set metatable
    RDTexture **texture =  (RDTexture **)lua_newuserdata(L, sizeof(RDTexture *));
    *texture = t;
    luaL_getmetatable(L, RD_LIB_TEXTURE);
    lua_setmetatable(L, -2);
    return 1;// number of return values
}

static int lnewTTFTexture(lua_State * L)
{
#if defined(RD_DEBUG)
    // chehck args
    int argc = lua_gettop(L);
    if (argc != 8) {
        return luaL_error(L, "[Lua Error]: %s new() fail, has wrong number of arguments", RD_LIB_TEXTURE);
    }
#endif
    const char *text = lua_tostring(L, 1);
    const char *font = lua_tostring(L, 2);
    float fontSize = lua_tonumber(L, 3);
    lua_Integer fR = lua_tointeger(L, 4);
    lua_Integer fG = lua_tointeger(L, 5);
    lua_Integer fB = lua_tointeger(L, 6);
    lua_Integer fA = lua_tointeger(L, 7);
    // create sprite
    RDColor fontColor;
    fontColor.r = fR;
    fontColor.g = fG;
    fontColor.b = fB;
    fontColor.a = fA;
    
    lua_Integer align = lua_tonumber(L, 8);
    
    RDTexture *t = RDTexture::createWithTTF(text, font, fontSize, &fontColor, (int)align);
    if (!t) {
        return luaL_error(L, "[Lua Error]: %s newTTF() fail, texture not found", RD_LIB_TEXTURE);
    }
    
    // create userdata and set metatable
    RDTexture **texture =  (RDTexture **)lua_newuserdata(L, sizeof(RDTexture *));
    *texture = t;
    luaL_getmetatable(L, RD_LIB_TEXTURE);
    lua_setmetatable(L, -2);
    return 1;// number of return values
}

static int lgc(lua_State *L)
{
    RDTexture **texture = (RDTexture **)luaL_checkudata(L, 1, RD_LIB_TEXTURE);
#if defined(RD_DEBUG)
    if (NULL == texture || NULL == *texture) {
        return luaL_error(L, "[Lua Error]: %s gc fail!", RD_LIB_TEXTURE);
    }
#endif

    delete *texture;
    return 0;// number of return values
}

static int lgetWidth(lua_State *L)
{
    RDTexture **texture = (RDTexture **)luaL_checkudata(L, 1, RD_LIB_TEXTURE);
#if defined(RD_DEBUG)
    if (NULL == texture || NULL == *texture) {
        return luaL_error(L, "[Lua Error]: %s getWidth fail!", RD_LIB_TEXTURE);
    }
#endif
    
    lua_pushnumber(L, (*texture)->getWidth());
    
    return 1;// number of return values
}

static int lgetHeight(lua_State *L)
{
    RDTexture **texture = (RDTexture **)luaL_checkudata(L, 1, RD_LIB_TEXTURE);
#if defined(RD_DEBUG)
    if (NULL == texture || NULL == *texture) {
        return luaL_error(L, "[Lua Error]: %s getHeight fail!", RD_LIB_TEXTURE);
    }
#endif
    
    lua_pushnumber(L, (*texture)->getHeight());
    
    return 1;// number of return values
}

static const struct luaL_Reg texture_meta [] = {
    {"getWidth", lgetWidth},
    {"getHeight", lgetHeight},
    {"__gc", lgc},
    {NULL, NULL}
};

static const struct luaL_Reg texture_funcs [] = {
    {"new", lnewTexture},
    {"newTTF", lnewTTFTexture},
    {NULL, NULL}
};

// extern func for register library
LUAMOD_API int luaopen_Texture(lua_State *L)
{
    if (luaL_newmetatable(L, RD_LIB_TEXTURE)) {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);  /* duplicates the metatable */
        lua_setfield(L, -2, "__index");
        /* add method to metatable */
        luaL_setfuncs(L, texture_meta, 0);
        lua_pop(L, 1);  /* pop new metatable */
        
        // binding userdata to new metatable
        luaL_newlib(L, texture_funcs);
        return 1;
    }
    return 0;
}
