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

#ifndef __RDLua_h__
#define __RDLua_h__

// lua5 header files
#include "lua.hpp"

#ifdef __cplusplus
extern "C" {
#endif

// Rapid2D bindings
LUAMOD_API int luaopen_cjson(lua_State *l);
#define RD_LIB_LUALOADER "Rapid2D_CLuaLoader"
LUAMOD_API int (luaopen_LuaLoader) (lua_State *L);
#define RD_LIB_SPRITE "Rapid2D_CSprite"
LUAMOD_API int (luaopen_Sprite) (lua_State *L);
#define RD_LIB_TEXTURE "Rapid2D_CTexture"
LUAMOD_API int luaopen_Texture(lua_State *L);
#define RD_LIB_SPRITEFRAME "Rapid2D_CSriteFrame"
LUAMOD_API int luaopen_SpriteFrame(lua_State *L);
#define RD_LIB_FILESYSTEM "Rapid2D_CFileSystem"
LUAMOD_API int luaopen_FileSystem(lua_State *L);
#define RD_LIB_MAT4 "Rapid2D_CMat4"
LUAMOD_API int luaopen_Mat4(lua_State *L);
#define RD_LIB_DRAWVERTEX "Rapid2D_CDrawVertex"
LUAMOD_API int (luaopen_DrawVertex) (lua_State *L);
#define RD_LIB_OPENGL "Rapid2D_COpenGL"
LUAMOD_API int (luaopen_OpenGL) (lua_State *L);
#define RD_LIB_AUDIO "Rapid2D_CAudio"
LUAMOD_API int luaopen_audio(lua_State *L);
#define RD_LIB_CRYPTO "Rapid2D_CCrypto"
LUAMOD_API int luaopen_crypto(lua_State *L);

// name is 'require' search path
int RDLuaLoadFromFile(lua_State *L, const char *name);
int RDLuaLoadFromChunks(lua_State *L, const char *fullPath, const char *key);
void RDLuaInit(const char *name, const char *key);
void RDLuaDraw(float dt);
void RDLuaTouchEvent(char *type, int count, intptr_t ids[], float xs[], float ys[]);
void RDLuaKeyBoardEvent(char *type, int keyCode);
void RDLuaClose(void);

#ifdef __cplusplus
}
#endif

#endif /* __RDLua_h__ */
