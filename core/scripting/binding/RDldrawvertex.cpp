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

#include "RDDrawVertex.h"
#include "RDLua.h"
#include "RDLog.h"

static int lnewDrawVertex(lua_State * L)
{
#if defined(RD_DEBUG)
    // check args
    int argc = lua_gettop(L);
    if (argc != 3) {
        return luaL_error(L, "[Lua Error]: %s new() fail, has wrong number of arguments",
                          RD_LIB_DRAWVERTEX);
    }
#endif

    size_t len = lua_rawlen(L, 1);
    RDBasicVertex *vertex = (RDBasicVertex *)malloc(sizeof(RDBasicVertex) * len);
    for (int i = 1; i <= len; i++) {
        lua_rawgeti(L, 1, i);// push vertex table
        
        lua_rawgeti(L, -1, 1);// get x
        vertex[i - 1].position.x = lua_tonumber(L, -1);
        lua_pop(L, 1);
        
        lua_rawgeti(L, -1, 2);// get y
        vertex[i - 1].position.y = lua_tonumber(L, -1);
        lua_pop(L, 1);
        
        lua_rawgeti(L, -1, 3);// get z
        vertex[i - 1].position.z = lua_tonumber(L, -1);
        lua_pop(L, 1);
        
        lua_rawgeti(L, -1, 4);// get r
        vertex[i - 1].color.r = lua_tointeger(L, -1) / 255.0; // to GLColor
        lua_pop(L, 1);
        
        lua_rawgeti(L, -1, 5);// get g
        vertex[i - 1].color.g = lua_tointeger(L, -1) / 255.0;
        lua_pop(L, 1);
        
        lua_rawgeti(L, -1, 6);// get b
        vertex[i - 1].color.b = lua_tointeger(L, -1) / 255.0;
        lua_pop(L, 1);
        
        lua_rawgeti(L, -1, 7);// get a
        vertex[i - 1].color.a = lua_tointeger(L, -1) / 255.0;
        lua_pop(L, 1);
        
        lua_pop(L, 1);// pop vertex table
    }
    
    int drawType = lua_tointeger(L, 2);
    lua_Number pointSize = lua_tonumber(L, 3);
    
    // create sprite
    RDDrawVertex *v = RDDrawVertex::create(vertex, len, drawType, pointSize);
    free(vertex);
    // create userdata and set metatable
    RDDrawVertex **drawVertex =  (RDDrawVertex **)lua_newuserdata(L, sizeof(RDDrawVertex *));
    *drawVertex = v;
    luaL_getmetatable(L, RD_LIB_DRAWVERTEX);
    lua_setmetatable(L, -2);
    return 1;// number of return values
}

/* ==== meta func ==== */
static int ldraw(lua_State *L)
{
    RDDrawVertex **drawVertex = (RDDrawVertex **)luaL_checkudata(L, 1, RD_LIB_DRAWVERTEX);
#if defined(RD_DEBUG)
    if (NULL == drawVertex || NULL == *drawVertex) {
        return luaL_error(L, "[Lua Error]: %s draw fail!", RD_LIB_DRAWVERTEX);
    }
#endif
    
    RDMat4 *absMat4 = (RDMat4 *)lua_touserdata(L, 2);
    (*drawVertex)->draw(absMat4);
    
    return 0;// number of return values
}

static int lgc(lua_State *L)
{
    RDDrawVertex **drawVertex = (RDDrawVertex **)luaL_checkudata(L, 1, RD_LIB_DRAWVERTEX);
#if defined(RD_DEBUG)
    if (NULL == drawVertex || NULL == *drawVertex) {
        return luaL_error(L, "[Lua Error]: %s gc fail!", RD_LIB_DRAWVERTEX);
    }
#endif

    delete *drawVertex;
    
    return 0;// number of return values
}

static const struct luaL_Reg drawVertex_meta [] = {
    {"draw", ldraw},
    {"__gc", lgc},
    {NULL, NULL}
};

static const struct luaL_Reg drawVertex_funcs [] = {
    {"new", lnewDrawVertex},
    {NULL, NULL}
};

// extern func for register library
LUAMOD_API int luaopen_DrawVertex(lua_State *L)
{
    if (luaL_newmetatable(L, RD_LIB_DRAWVERTEX)) {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);  /* duplicates the metatable */
        lua_setfield(L, -2, "__index");
        /* add method to metatable */
        luaL_setfuncs(L, drawVertex_meta, 0);
        lua_pop(L, 1);  /* pop new metatable */
        
        // binding userdata to new metatable
        luaL_newlib(L, drawVertex_funcs);
        return 1;
    }
    return 0;
}
