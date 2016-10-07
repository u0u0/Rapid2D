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

#include <string.h>

#include "RDMath.h"
#include "RDLua.h"
#include "RDLog.h"

static int lnewMat4(lua_State * L)
{
#if defined(RD_DEBUG)
    // check args
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "[Lua Error]: %s new() fail, has wrong number of arguments", RD_LIB_MAT4);
    }
#endif
    
    // create userdata and set metatable
    RDMat4 *mat4 =  (RDMat4 *)lua_newuserdata(L, sizeof(RDMat4));
    memset(mat4, 0, sizeof(RDMat4));
    
    return 1;// number of return values
}

static int lmodelView(lua_State * L)
{
    RDMat4 *mat4 = (RDMat4 *)lua_touserdata(L, 1);
    
    // pos table
    RDVec3 pos;
    lua_pushstring(L, "x");
    lua_rawget(L, 2);
    pos.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_pushstring(L, "y");
    lua_rawget(L, 2);
    pos.y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_pushstring(L, "z");
    lua_rawget(L, 2);
    pos.z = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    // rotation table
    RDVec3 rotation;
    lua_pushstring(L, "x");
    lua_rawget(L, 3);
    rotation.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_pushstring(L, "y");
    lua_rawget(L, 3);
    rotation.y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_pushstring(L, "z");
    lua_rawget(L, 3);
    rotation.z = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    // scale table
    RDVec3 scale;
    lua_pushstring(L, "x");
    lua_rawget(L, 4);
    scale.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_pushstring(L, "y");
    lua_rawget(L, 4);
    scale.y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_pushstring(L, "z");
    lua_rawget(L, 4);
    scale.z = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    RDMath_modelView(mat4, &pos, &rotation, &scale);
    return 0;
}

static int lmultiply(lua_State * L)
{
    RDMat4 *out = (RDMat4 *)lua_touserdata(L, 1);
    RDMat4 *abs = (RDMat4 *)lua_touserdata(L, 2);
    RDMat4 *relative = (RDMat4 *)lua_touserdata(L, 3);
    
    RDMat4Multiply(out, abs, relative);
    return 0;
}

static int ltranslation(lua_State * L)
{
    RDMat4 *mat4 = (RDMat4 *)lua_touserdata(L, 1);
    RDVec3 translation;
    RDMat4extractTranslation(&translation, mat4);
    
    lua_newtable(L);
    lua_pushstring(L, "x");
    lua_pushnumber(L, translation.x);
    lua_rawset(L, -3);
    lua_pushstring(L, "y");
    lua_pushnumber(L, translation.y);
    lua_rawset(L, -3);
    lua_pushstring(L, "z");
    lua_pushnumber(L, translation.z);
    lua_rawset(L, -3);
    
    return 1;
}

static int lrotation(lua_State * L)
{
    RDMat4 *mat4 = (RDMat4 *)lua_touserdata(L, 1);
    RDVec3 rotation;
    RDMat4extractRotation(&rotation, mat4);
    
    lua_newtable(L);
    lua_pushstring(L, "x");
    lua_pushnumber(L, rotation.x);
    lua_rawset(L, -3);
    lua_pushstring(L, "y");
    lua_pushnumber(L, rotation.y);
    lua_rawset(L, -3);
    lua_pushstring(L, "z");
    lua_pushnumber(L, rotation.z);
    lua_rawset(L, -3);
    
    return 1;
}

static int lscale(lua_State * L)
{
    RDMat4 *mat4 = (RDMat4 *)lua_touserdata(L, 1);
    RDVec3 scale;
    RDMat4extractScale(&scale, mat4);
    
    lua_newtable(L);
    lua_pushstring(L, "x");
    lua_pushnumber(L, scale.x);
    lua_rawset(L, -3);
    lua_pushstring(L, "y");
    lua_pushnumber(L, scale.y);
    lua_rawset(L, -3);
    lua_pushstring(L, "z");
    lua_pushnumber(L, scale.z);
    lua_rawset(L, -3);
    
    return 1;
}

static int lpointToNodeSpace(lua_State * L)
{
    RDMat4 *mat4 = (RDMat4 *)lua_touserdata(L, 1);
    // get point for arg 2
    RDVec3 point;
    point.z = 0;
    lua_pushstring(L, "x");
    lua_rawget(L, 2);
    point.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_pushstring(L, "y");
    lua_rawget(L, 2);
    point.y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    RDMat4 inversed;
    RDMat4Inversed(&inversed, mat4);
    
    RDVec3 out;
    RDMat4TransformVec3(&out, &inversed, &point);
    
    // return out point {x = x, y = y}
    lua_newtable(L);
    lua_pushstring(L, "x");
    lua_pushnumber(L, out.x);
    lua_rawset(L, -3);
    lua_pushstring(L, "y");
    lua_pushnumber(L, out.y);
    lua_rawset(L, -3);
    return 1;
}

static int lpointToWorldSpace(lua_State * L)
{
    RDMat4 *mat4 = (RDMat4 *)lua_touserdata(L, 1);
    // get point for arg 2
    RDVec3 point;
    point.z = 0;
    lua_pushstring(L, "x");
    lua_rawget(L, 2);
    point.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_pushstring(L, "y");
    lua_rawget(L, 2);
    point.y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    RDVec3 out;
    RDMat4TransformVec3(&out, mat4, &point);
    
    // return out point {x = x, y = y}
    lua_newtable(L);
    lua_pushstring(L, "x");
    lua_pushnumber(L, out.x);
    lua_rawset(L, -3);
    lua_pushstring(L, "y");
    lua_pushnumber(L, out.y);
    lua_rawset(L, -3);
    return 1;
}

static const struct luaL_Reg mat4_funcs [] = {
    {"new", lnewMat4},
    {"modelView", lmodelView},
    {"multiply", lmultiply},
    {"extractTranslation", ltranslation},
    {"extractRotation", lrotation},
    {"extractScale", lscale},
    {"pointToNodeSpace", lpointToNodeSpace},
    {"pointToWorldSpace", lpointToWorldSpace},
    {NULL, NULL}
};

// extern func for register library
LUAMOD_API int luaopen_Mat4(lua_State *L)
{
    // binding userdata to new metatable
    luaL_newlib(L, mat4_funcs);
    return 1;
}
