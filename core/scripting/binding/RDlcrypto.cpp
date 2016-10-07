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

#include "RDLua.h"
#include "RDLog.h"

#include "md5.h"
#include "xxtea.h"
#include "base64.h"

// need free by caller
static char *bin2hex(unsigned char *bin, int binLength)
{
    static const char *hextable = "0123456789abcdef";
    
    int hexLength = binLength * 2 + 1;
    char *hex = (char *)malloc(sizeof(char) * hexLength);
    memset(hex, 0, sizeof(char) * hexLength);
    
    int ci = 0;
    for (int i = 0; i < 16; ++i) {
        unsigned char c = bin[i];
        hex[ci++] = hextable[(c >> 4) & 0x0f];
        hex[ci++] = hextable[c & 0x0f];
    }
    
    return hex;
}

static int lmd5(lua_State *L)
{
    // check args
#if defined(RD_DEBUG)
    int argc = lua_gettop(L);
    if (argc != 2) {
        return luaL_error(L, "[Lua Error]: %s md5() fail, has wrong number of arguments",
                          RD_LIB_CRYPTO);
    }
#endif
    
    const char *buffer = lua_tostring(L, 1);
    size_t size = lua_rawlen(L, 1);
    int isRawOutput = lua_toboolean(L, 2);
    
    unsigned char output[16] = {0};
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, buffer, size);
    MD5_Final(output, &ctx);
    
    if (isRawOutput) {
        lua_pushlstring(L, (const char *)output, sizeof(output));
    } else {
        char *hex = bin2hex(output, sizeof(output));
        lua_pushstring(L, hex);
        free(hex);
    }
    
    return 1;
}

static int lencryptXXTEA(lua_State *L)
{
    // check args
#if defined(RD_DEBUG)
    int argc = lua_gettop(L);
    if (argc != 2) {
        return luaL_error(L, "[Lua Error]: %s encryptXXTEA() fail, has wrong number of arguments",
                          RD_LIB_CRYPTO);
    }
#endif
    
    const char *buffer = lua_tostring(L, 1);
    size_t size = lua_rawlen(L, 1);
    const char *key = lua_tostring(L, 2);
    
    size_t outLen = 0;
    void *output = xxtea_encrypt(buffer, size, key, &outLen);
    if (output) {
        lua_pushlstring(L, (const char *)output, outLen);
        free(output);
        return 1;
    }
    
    return 0;
}

static int ldecryptXXTEA(lua_State *L)
{
    // check args
#if defined(RD_DEBUG)
    int argc = lua_gettop(L);
    if (argc != 2) {
        return luaL_error(L, "[Lua Error]: %s decryptXXTEA() fail, has wrong number of arguments",
                          RD_LIB_CRYPTO);
    }
#endif
    
    const char *buffer = lua_tostring(L, 1);
    size_t size = lua_rawlen(L, 1);
    const char *key = lua_tostring(L, 2);
    
    size_t outLen = 0;
    void *output = xxtea_decrypt(buffer, size, key, &outLen);
    if (output) {
        lua_pushlstring(L, (const char *)output, outLen);
        free(output);
        return 1;
    }
    
    return 0;
}

static int lencodeBase64(lua_State *L)
{
    // check args
#if defined(RD_DEBUG)
    int argc = lua_gettop(L);
    if (argc != 1) {
        return luaL_error(L, "[Lua Error]: %s encodeBase64() fail, has wrong number of arguments",
                          RD_LIB_CRYPTO);
    }
#endif
    
    const char *buffer = lua_tostring(L, 1);
    size_t size = lua_rawlen(L, 1);
    
    int outlen = Base64encode_len(size);
    char *outBuff = (char *)malloc(outlen);
    Base64encode(outBuff, buffer, size);
    // Base64encode auto add '\0' at string end
    lua_pushstring(L, outBuff);
    free(outBuff);
    return 1;
}

static int ldecodeBase64(lua_State *L)
{
    // check args
#if defined(RD_DEBUG)
    int argc = lua_gettop(L);
    if (argc != 1) {
        return luaL_error(L, "[Lua Error]: %s decodeBase64() fail, has wrong number of arguments",
                          RD_LIB_CRYPTO);
    }
#endif
    
    const char *buffer = lua_tostring(L, 1);
    
    int outlen = Base64decode_len(buffer);
    char *outBuff = (char *)malloc(outlen);
    // refix outlen by return value
    outlen = Base64decode(outBuff, buffer);
    // binary string
    lua_pushlstring(L, outBuff, outlen);
    free(outBuff);
    return 1;
}

static const struct luaL_Reg luafs_funcs [] = {
    {"md5", lmd5},
    {"encryptXXTEA", lencryptXXTEA},
    {"decryptXXTEA", ldecryptXXTEA},
    {"encodeBase64", lencodeBase64},
    {"decodeBase64", ldecodeBase64},
    {NULL, NULL}
};

// extern func for register library
LUAMOD_API int luaopen_crypto(lua_State *L)
{
    luaL_newlib(L, luafs_funcs);
    return 1;
}
