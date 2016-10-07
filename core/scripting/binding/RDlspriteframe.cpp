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

#include "RDSpriteFrame.h"
#include "RDPortingTypes.h"
#include "RDLua.h"
#include "RDLog.h"

static int lnewFrame(lua_State * L)
{
#if defined(RD_DEBUG)
    // check args
    int argc = lua_gettop(L);
    if (argc != 14) {
        return luaL_error(L, "[Lua Error]: %s new() fail, has wrong number of arguments", RD_LIB_SPRITEFRAME);
    }
#endif
    
    // create userdata and set metatable
    RDSpriteFrame *frame =  (RDSpriteFrame *)lua_newuserdata(L, sizeof(RDSpriteFrame));
    memset(frame, 0, sizeof(RDSpriteFrame));
    
    // param 1. get name
#if defined(RD_DEBUG)
    const char *name = lua_tostring(L, 1);
    frame->name = strdup(name);
#endif
    // param 2. get rotated
    frame->rotated = lua_toboolean(L, 2);
    // param 3. get trimmed
    frame->trimmed = lua_toboolean(L, 3);
    // param 4～7. get frame
    frame->frame.origin.x = lua_tonumber(L, 4);
    frame->frame.origin.y = lua_tonumber(L, 5);
    frame->frame.size.width = lua_tonumber(L, 6);
    frame->frame.size.height = lua_tonumber(L, 7);
    // param 8～11. get source
    frame->source.origin.x = lua_tonumber(L, 8);
    frame->source.origin.y = lua_tonumber(L, 9);
    frame->source.size.width = lua_tonumber(L, 10);
    frame->source.size.height = lua_tonumber(L, 11);
    // param 12~13. get source Size
    frame->sourceSize.width = lua_tonumber(L, 12);
    frame->sourceSize.height = lua_tonumber(L, 13);
    // param 14. get texture
    RDTexture **texture = (RDTexture **)luaL_checkudata(L, 14, RD_LIB_TEXTURE);
    frame->texture = *texture;
    
    luaL_getmetatable(L, RD_LIB_SPRITEFRAME);
    lua_setmetatable(L, -2);
    return 1;// number of return values
}

static int lgc(lua_State *L)
{
#if defined(RD_DEBUG)
    RDSpriteFrame *frame = (RDSpriteFrame *)luaL_checkudata(L, 1, RD_LIB_SPRITEFRAME);
    if (NULL == frame) {
        return luaL_error(L, "[Lua Error]: %s gc fail!", RD_LIB_SPRITEFRAME);
    }
    free(frame->name);
#endif
    return 0;// number of return values
}

static const struct luaL_Reg spriteFrame_meta [] = {
    {"__gc", lgc},
    {NULL, NULL}
};

static const struct luaL_Reg spriteFrame_funcs [] = {
    {"new", lnewFrame},
    {NULL, NULL}
};

// extern func for register library
LUAMOD_API int luaopen_SpriteFrame(lua_State *L)
{
    if (luaL_newmetatable(L, RD_LIB_SPRITEFRAME)) {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);  /* duplicates the metatable */
        lua_setfield(L, -2, "__index");
        /* add method to metatable */
        luaL_setfuncs(L, spriteFrame_meta, 0);
        lua_pop(L, 1);  /* pop new metatable */
        
        // binding userdata to new metatable
        luaL_newlib(L, spriteFrame_funcs);
        return 1;
    }
    return 0;
}
