#include <lauxlib.h>
#include <lua.h>
#include <stdlib.h>
#include <zlib.h>
#include <string.h>

#define BUFF_SIZE 64

static int load(lua_State *L) {
    const char *file_path = luaL_checkstring(L, 1);

    gzFile file;
    file = gzopen(file_path, "r");
    if (!file) {
        lua_pushnil(L);
        return 1;
    }

    char *content = NULL;
    int len = 0;

    while (1) {
        int err, bytes_read;
        unsigned char buff[BUFF_SIZE];
        bytes_read = gzread(file, buff, BUFF_SIZE);
        content = realloc(content, len + bytes_read);
        memcpy(content + len, buff, bytes_read);
        len += bytes_read;
        if (bytes_read < BUFF_SIZE) {
            if(gzeof(file)){
                break;
            }
            luaL_error(L, "Something went wrong: %s", gzerror(file, &err));
        }
    }

    lua_pushlstring(L, content, len);
    free(content);
    gzclose(file);
    return 1;
}

int luaopen_gzlua(lua_State *L) {
    static const struct luaL_Reg lib[] = {{"load", load}, {NULL, NULL}};
#if LUA_VERSION_NUM >= 502
    luaL_newlib(L, lib);
#else
    luaL_register(L, "gzlua", lib);
#endif
    return 1;
}
