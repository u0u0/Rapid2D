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

#include "RDLua.h"
#include "RDTypes.h"
#include "RDLog.h"

static int lclearColor(lua_State *L)
{
    lua_pushstring(L, "r");
    lua_rawget(L, 1);
    GLfloat r = lua_tointeger(L, -1) / 255.0;
    lua_pop(L, 1);
    
    lua_pushstring(L, "g");
    lua_rawget(L, 1);
    GLfloat g = lua_tointeger(L, -1) / 255.0;
    lua_pop(L, 1);
    
    lua_pushstring(L, "b");
    lua_rawget(L, 1);
    GLfloat b = lua_tointeger(L, -1) / 255.0;
    lua_pop(L, 1);
    
    lua_pushstring(L, "a");
    lua_rawget(L, 1);
    GLfloat a = lua_tointeger(L, -1) / 255.0;
    lua_pop(L, 1);
    
    glClearColor(r, g, b, a);
    return 0;
}

static const struct luaL_Reg luaOpenGL_funcs [] = {
    {"clearColor", lclearColor},
    {NULL, NULL}
};

// extern func for register library
LUAMOD_API int luaopen_OpenGL(lua_State *L)
{
    luaL_newlib(L, luaOpenGL_funcs);
    return 1;
}
