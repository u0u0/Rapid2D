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

#include "RDSprite.h"
#include "RDLua.h"
#include "RDLog.h"

static int lnewSprite(lua_State * L)
{
    // check args
#if defined(RD_DEBUG)
    int argc = lua_gettop(L);
    if (argc != 1) {
        return luaL_error(L, "[Lua Error]: %s new() fail, has wrong number of arguments", RD_LIB_SPRITE);
    }
#endif
    RDSpriteFrame *frame = (RDSpriteFrame *)luaL_checkudata(L, 1, RD_LIB_SPRITEFRAME);
#if defined(RD_DEBUG)
    if (!frame) {
        return luaL_error(L, "[Lua Error]: %s new() fail, frame is nil", RD_LIB_SPRITE);
    }
#endif

    // create sprite
    RDSprite *s = RDSprite::create(frame);
    
    // create userdata and set metatable
    RDSprite **sprite =  (RDSprite **)lua_newuserdata(L, sizeof(RDSprite *));
    *sprite = s;
    luaL_getmetatable(L, RD_LIB_SPRITE);
    lua_setmetatable(L, -2);
    return 1;// number of return values
}

/* ==== meta func ==== */
static int ldraw(lua_State *L)
{
    RDSprite **sprite = (RDSprite **)luaL_checkudata(L, 1, RD_LIB_SPRITE);
#if defined(RD_DEBUG)
    if (NULL == sprite || NULL == *sprite) {
        return luaL_error(L, "[Lua Error]: %s draw fail!", RD_LIB_SPRITE);
    }
#endif
    
    RDMat4 *absMat4 = (RDMat4 *)lua_touserdata(L, 2);
    (*sprite)->draw(absMat4);
    
    return 0;// number of return values
}

static int lsetAnchorPoint(lua_State *L)
{
    RDSprite **sprite = (RDSprite **)luaL_checkudata(L, 1, RD_LIB_SPRITE);
#if defined(RD_DEBUG)
    if (NULL == sprite || NULL == *sprite) {
        return luaL_error(L, "[Lua Error]: %s setAnchorPoint fail!", RD_LIB_SPRITE);
    }
    // check args
    int argc = lua_gettop(L);
    if (argc != 3) {
        return luaL_error(L, "[Lua Error]: %s setAnchorPoint fail, has wrong number of arguments", RD_LIB_SPRITE);
    }
    
    if (!lua_isnumber(L, 2) || !lua_isnumber(L, 3)) {
        return luaL_error(L, "[Lua Error]: %s setAnchorPoint need number", RD_LIB_SPRITE);
    }
#endif
    
    RDPoint point;
    point.x = lua_tonumber(L, 2);
    point.y = lua_tonumber(L, 3);
#if defined(RD_DEBUG)
    if (point.x > 1.0 || point.x < 0.0 || point.y > 1.0 || point.y < 0.0) {
        return luaL_error(L, "[Lua Error]: %s setAnchorPoint, argumengts range [0.0, 1.0]", RD_LIB_SPRITE);
    }
#endif
    (*sprite)->setAnchorPoint(&point);
    
    return 0;// number of return values
}

static int lgetAnchorPoint(lua_State *L)
{
    RDSprite **sprite = (RDSprite **)luaL_checkudata(L, 1, RD_LIB_SPRITE);
#if defined(RD_DEBUG)
    if (NULL == sprite || NULL == *sprite) {
        return luaL_error(L, "[Lua Error]: %s getAnchorPoint fail!", RD_LIB_SPRITE);
    }
#endif
    
    RDPoint *point = (*sprite)->getAnchorPoint();
    lua_pushnumber(L, point->x);
    lua_pushnumber(L, point->y);
    return 2;// number of return values
}

static int lsetColor(lua_State *L)
{
    RDSprite **sprite = (RDSprite **)luaL_checkudata(L, 1, RD_LIB_SPRITE);
#if defined(RD_DEBUG)
    if (NULL == sprite || NULL == *sprite) {
        return luaL_error(L, "[Lua Error]: %s setColor fail!", RD_LIB_SPRITE);
    }
    // check args
    int argc = lua_gettop(L);
    if (argc != 5) {
        return luaL_error(L, "[Lua Error]: %s setColor fail, has wrong number of arguments", RD_LIB_SPRITE);
    }
    
    if (!lua_isinteger(L, 2) || !lua_isinteger(L, 3) || !lua_isinteger(L, 4) || !lua_isinteger(L, 5)) {
        return luaL_error(L, "[Lua Error]: %s setColor need integer", RD_LIB_SPRITE);
    }
#endif
    
    RDColor color;
    color.r = lua_tointeger(L, 2);
    color.g = lua_tointeger(L, 3);
    color.b = lua_tointeger(L, 4);
    color.a = lua_tointeger(L, 5);

    (*sprite)->setColor(&color);
    return 0;// number of return values
}

static int lgetColor(lua_State *L)
{
    RDSprite **sprite = (RDSprite **)luaL_checkudata(L, 1, RD_LIB_SPRITE);
#if defined(RD_DEBUG)
    if (NULL == sprite || NULL == *sprite) {
        return luaL_error(L, "[Lua Error]: %s getColor fail!", RD_LIB_SPRITE);
    }
#endif
    
    RDColor *color = (*sprite)->getColor();
    lua_pushinteger(L, color->r);
    lua_pushinteger(L, color->g);
    lua_pushinteger(L, color->b);
    lua_pushinteger(L, color->a);
    return 4;// number of return values
}

static int lsetFlip(lua_State *L)
{
    RDSprite **sprite = (RDSprite **)luaL_checkudata(L, 1, RD_LIB_SPRITE);
#if defined(RD_DEBUG)
    if (NULL == sprite || NULL == *sprite) {
        return luaL_error(L, "[Lua Error]: %s setFlip fail!", RD_LIB_SPRITE);
    }
    // check args
    int argc = lua_gettop(L);
    if (argc != 3) {
        return luaL_error(L, "[Lua Error]: %s setFlip fail, has wrong number of arguments", RD_LIB_SPRITE);
    }
    
    if (!lua_isboolean(L, 2) || !lua_isboolean(L, 3)) {
        return luaL_error(L, "[Lua Error]: %s setFlip need boolean", RD_LIB_SPRITE);
    }
#endif
    
    int flipx = lua_toboolean(L, 2);
    int flipy = lua_toboolean(L, 3);
    (*sprite)->setFlip(flipx, flipy);
    return 0;// number of return values
}

static int lgetFlip(lua_State *L)
{
    RDSprite **sprite = (RDSprite **)luaL_checkudata(L, 1, RD_LIB_SPRITE);
#if defined(RD_DEBUG)
    if (NULL == sprite || NULL == *sprite) {
        return luaL_error(L, "[Lua Error]: %s getFlipX fail!", RD_LIB_SPRITE);
    }
#endif
    
    bool flipx = (*sprite)->getFlipX();
    bool flipy = (*sprite)->getFlipY();
    lua_pushboolean(L, flipx);
    lua_pushboolean(L, flipy);
    return 2;// number of return values
}

static int lsetOpacity(lua_State *L)
{
    RDSprite **sprite = (RDSprite **)luaL_checkudata(L, 1, RD_LIB_SPRITE);
#if defined(RD_DEBUG)
    if (NULL == sprite || NULL == *sprite) {
        return luaL_error(L, "[Lua Error]: %s setOpacity fail!", RD_LIB_SPRITE);
    }
    // check args
    int argc = lua_gettop(L);
    if (argc != 2) {
        return luaL_error(L, "[Lua Error]: %s setOpacity fail, has wrong number of arguments", RD_LIB_SPRITE);
    }
    
    if (!lua_isinteger(L, 2)) {
        return luaL_error(L, "[Lua Error]: %s setOpacity need integer", RD_LIB_SPRITE);
    }
#endif
    
    lua_Integer opacity = lua_tointeger(L, 2);
#if defined(RD_DEBUG)
    if (opacity > 255 || opacity < 0) {
        return luaL_error(L, "[Lua Error]: %s setopacity, argumengts range [0, 255]", RD_LIB_SPRITE);
    }
#endif
    (*sprite)->setOpacity(opacity);
    return 0;// number of return values
}

static int lgetOpacity(lua_State *L)
{
    RDSprite **sprite = (RDSprite **)luaL_checkudata(L, 1, RD_LIB_SPRITE);
#if defined(RD_DEBUG)
    if (NULL == sprite || NULL == *sprite) {
        return luaL_error(L, "[Lua Error]: %s getOpacity fail!", RD_LIB_SPRITE);
    }
#endif
    
    RD_uchar opacity = (*sprite)->getOpacity();
    lua_pushinteger(L, opacity);
    return 1;// number of return values
}

static int lsetFrame(lua_State *L)
{
    RDSprite **sprite = (RDSprite **)luaL_checkudata(L, 1, RD_LIB_SPRITE);
#if defined(RD_DEBUG)
    if (NULL == sprite || NULL == *sprite) {
        return luaL_error(L, "[Lua Error]: %s setFrame fail!", RD_LIB_SPRITE);
    }
    // check args
    int argc = lua_gettop(L);
    if (argc != 2) {
        return luaL_error(L, "[Lua Error]: %s setFrame fail, has wrong number of arguments", RD_LIB_SPRITE);
    }
#endif
    
    RDSpriteFrame *frame = (RDSpriteFrame *)luaL_checkudata(L, 2, RD_LIB_SPRITEFRAME);
#if defined(RD_DEBUG)
    if (NULL == frame) {
        return luaL_error(L, "[Lua Error]: %s setFrame fail!, params must cframe", RD_LIB_SPRITE);
    }
#endif
    
    (*sprite)->setFrame(frame);
    return 0;// number of return values
}

static int lgc(lua_State *L)
{
    RDSprite **sprite = (RDSprite **)luaL_checkudata(L, 1, RD_LIB_SPRITE);
#if defined(RD_DEBUG)
    if (NULL == sprite || NULL == *sprite) {
        return luaL_error(L, "[Lua Error]: %s gc fail!", RD_LIB_SPRITE);
    }
#endif

    delete *sprite;
    
    return 0;// number of return values
}

static const struct luaL_Reg sprite_meta [] = {
    {"draw", ldraw},
    {"setAnchorPoint", lsetAnchorPoint},
    {"getAnchorPoint", lgetAnchorPoint},
    {"setColor", lsetColor},
    {"getColor", lgetColor},
    {"setFlip", lsetFlip},
    {"getFlip", lgetFlip},
    {"setOpacity", lsetOpacity},
    {"getOpacity", lgetOpacity},
    {"setFrame", lsetFrame},
    {"__gc", lgc},
    {NULL, NULL}
};

static const struct luaL_Reg sprite_funcs [] = {
    {"new", lnewSprite},
    {NULL, NULL}
};

// extern func for register library
LUAMOD_API int luaopen_Sprite(lua_State *L)
{
    if (luaL_newmetatable(L, RD_LIB_SPRITE)) {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);  /* duplicates the metatable */
        lua_setfield(L, -2, "__index");
        /* add method to metatable */
        luaL_setfuncs(L, sprite_meta, 0);
        lua_pop(L, 1);  /* pop new metatable */
        
        // binding userdata to new metatable
        luaL_newlib(L, sprite_funcs);
        return 1;
    }
    return 0;
}
