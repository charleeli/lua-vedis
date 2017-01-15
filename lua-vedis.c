#include <stdio.h>
#include <stdlib.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "vedis.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define SETLITERAL(n,v) (lua_pushliteral(L, n), lua_pushliteral(L, v), lua_settable(L, -3))
#define SETINT(n,v) (lua_pushliteral(L, n), lua_pushinteger(L, v), lua_settable(L, -3))

static void fatal(lua_State* L, vedis *db, const char *zMsg)
{
	if( db ){
		const char *zErr;
		int iLen = 0; /* Stupid cc warning */

		/* Extract the datastore error log */
		vedis_config(db, VEDIS_CONFIG_ERR_LOG, &zErr, &iLen);
		if( iLen > 0 ){
			/* Output the error log */
			luaL_error(L, zErr); /* Always null termniated */
		}
	}else{
		if( zMsg ){
			luaL_error(L, zMsg);
		}
	}
}

static int _vedis_open(lua_State* L)
{
    vedis *db;

    const char* file_name = luaL_checkstring(L, 1);
    /*unsigned int i_mode = luaL_optinteger(L, 2, 0);

    if (i_mode == 0)
        i_mode = VEDIS_OPEN_CREATE | VEDIS_OPEN_READWRITE;*/

    int rc = vedis_open(&db, file_name);
	if( rc != VEDIS_OK ){
		fatal(L, db, "Out of memory");
		lua_pushboolean(L,0);
        return 0;
	}
	else
    {
        lua_pushlightuserdata(L, db);
        return 1;
    }
}

static int _vedis_close(lua_State* L)
{
    vedis* db = (vedis*)lua_touserdata(L, 1);
    if (NULL == db)
    {
        luaL_argerror(L, 1,  "vedis: close get null db");
        lua_pushboolean(L,0);
        return 0;
    }

    int rs = vedis_close(db);
    if (rs != VEDIS_OK)
    {
        fatal(L, db, "unqlite: out of memory");
        lua_pushboolean(L,0);
        return 0;
    }
    else
    {
        lua_pushboolean(L,1);
        return 1;
    }
}

static int _vedis_kv_store(lua_State* L)
{
    vedis* db = (vedis*)lua_touserdata(L, 1);
    if (NULL == db)
    {
        luaL_argerror(L, 1,  "vedis: close get null db");
        lua_pushboolean(L,0);
        return 0;
    }

    size_t keylen;
    void* key = (void* )luaL_checklstring(L, 2, &keylen);
    size_t contentlen;
    void* content = (void* )luaL_checklstring(L, 3, &contentlen);

    if (keylen <= 0 || contentlen <= 0 || key == NULL || content == NULL)
    {
        luaL_argerror(L, 2, "vedis: invalid key or content");
        lua_pushboolean(L,0);
        return 0;
    }

    int rs = vedis_kv_store(db, key, keylen, content, contentlen);

    if (rs != VEDIS_OK)
    {
        fatal(L, db, "vedis: k-v store failed");
        lua_pushboolean(L,0);
        return 0;
    }
    else
    {
        lua_pushboolean(L,1);
        return 1;
    }
}

static int _vedis_kv_append(lua_State* L)
{
    vedis* db = (vedis*)lua_touserdata(L, 1);
    if (NULL == db)
    {
        luaL_argerror(L, 1,  "vedis: close get null db");
        lua_pushboolean(L,0);
        return 0;
    }

    size_t keylen;
    void* key = (void* )luaL_checklstring(L, 2, &keylen);
    size_t contentlen;
    void* content = (void* )luaL_checklstring(L, 3, &contentlen);

    if (keylen <= 0 || contentlen <= 0 || key == NULL || content == NULL)
    {
        luaL_argerror(L, 2, "vedis: invalid key or content");
        lua_pushboolean(L,0);
        return 0;
    }

    int rs = vedis_kv_append(db, key, keylen, content, contentlen);

    if (rs != VEDIS_OK)
    {
        fatal(L, db, "vedis: out of memory");
        lua_pushboolean(L,0);
        return 0;
    }
    else
    {
        lua_pushboolean(L,1);
        return 1;
    }
}

static int _vedis_kv_fetch(lua_State* L)
{
    vedis* db = (vedis*)lua_touserdata(L, 1);
    if (NULL == db)
    {
        luaL_argerror(L, 1,  "vedis: close get null db");
        lua_pushboolean(L,0);
        return 0;
    }

    size_t keylen;
    void* key = (void* )luaL_checklstring(L, 2, &keylen);

    if (keylen <= 0 || key == NULL )
    {
        luaL_argerror(L, 2, "vedis: invalid key");
        lua_pushboolean(L,0);
        return 0;
    }

    vedis_int64 buflen = 0;
    vedis_kv_fetch(db, key, keylen, NULL, &buflen);

    char* buf = (char *)malloc(sizeof(char)*buflen);
    vedis_kv_fetch(db, key, keylen, buf, &buflen);

    if (buf == NULL)
    {
        printf("vedis: nil buf");
        lua_pushnil(L);
    }
    else
    {
        lua_pushlstring(L, (const char *)buf, buflen);
        free(buf);
    }

    return 1;
}

static int _vedis_kv_delete(lua_State* L)
{
    vedis* db = (vedis*)lua_touserdata(L, 1);
    if (NULL == db)
    {
        luaL_argerror(L, 1,  "vedis: close get null db");
        lua_pushboolean(L,0);
        return 0;
    }

    size_t keylen;
    void* key = (void* )luaL_checklstring(L, 2, &keylen);

    if (keylen <= 0 || key == NULL )
    {
        luaL_argerror(L, 2, "vedis: invalid key");
        lua_pushboolean(L,0);
        return 0;
    }

    int rs = vedis_kv_delete(db, key, keylen);

    if (rs != VEDIS_OK)
    {
        fatal(L, db, "vedis: out of memory");
        lua_pushboolean(L,0);
        return 0;
    }
    else
    {
        lua_pushboolean(L,1);
        return 1;
    }
}

static int _vedis_begin(lua_State* L)
{
    vedis* db = (vedis*)lua_touserdata(L, 1);
    if (NULL == db)
    {
        luaL_argerror(L, 1,  "vedis: close get null db");
        lua_pushboolean(L,0);
        return 0;
    }

    int rs = vedis_begin(db);
    if (rs != VEDIS_OK)
    {
        fatal(L, db, "vedis: out of memory");
        lua_pushboolean(L,0);
        return 0;
    }
    else
    {
        lua_pushboolean(L,1);
        return 1;
    }
}

static int _vedis_commit(lua_State* L)
{
    vedis* db = (vedis*)lua_touserdata(L, 1);
    if (NULL == db)
    {
        luaL_argerror(L, 1,  "vedis: close get null db");
        lua_pushboolean(L,0);
        return 0;
    }

    int rs = vedis_commit(db);
    if (rs != VEDIS_OK)
    {
        fatal(L, db, "vedis: out of memory");
        lua_pushboolean(L,0);
        return 0;
    }
    else
    {
        lua_pushboolean(L,1);
        return 1;
    }
}

static int _vedis_rollback(lua_State* L)
{
    vedis* db = (vedis*)lua_touserdata(L, 1);
    if (NULL == db)
    {
        luaL_argerror(L, 1,  "vedis: close get null db");
        lua_pushboolean(L,0);
        return 0;
    }

    int rs = vedis_rollback(db);
    if (rs != VEDIS_OK)
    {
        fatal(L, db, "vedis: out of memory");
        lua_pushboolean(L,0);
        return 0;
    }
    else
    {
        lua_pushboolean(L,1);
        return 1;
    }
}

static const luaL_Reg vedis_functions[] =
{
    { "open", _vedis_open },
    { "close", _vedis_close },
    { "store", _vedis_kv_store },
    { "fetch", _vedis_kv_fetch },
    { "delete", _vedis_kv_delete },
    { "begin", _vedis_begin },
    { "commit", _vedis_commit },
    { NULL, NULL}
};

int luaopen_vedis(lua_State* L)
{
    luaL_newlib(L, vedis_functions);

    /*SETINT("VEDIS_OPEN_READONLY", VEDIS_OPEN_READONLY);
    SETINT("VEDIS_OPEN_READWRITE", VEDIS_OPEN_READWRITE);
    SETINT("VEDIS_OPEN_CREATE", VEDIS_OPEN_CREATE);
    SETINT("VEDIS_OPEN_EXCLUSIVE", VEDIS_OPEN_EXCLUSIVE);
    SETINT("VEDIS_OPEN_TEMP_DB", VEDIS_OPEN_TEMP_DB);
    SETINT("VEDIS_OPEN_NOMUTEX", VEDIS_OPEN_NOMUTEX);
    SETINT("VEDIS_OPEN_OMIT_JOURNALING", VEDIS_OPEN_OMIT_JOURNALING);
    SETINT("VEDIS_OPEN_IN_MEMORY", VEDIS_OPEN_IN_MEMORY);
    SETINT("VEDIS_OPEN_MMAP", VEDIS_OPEN_MMAP);*/
    return 1;
}
