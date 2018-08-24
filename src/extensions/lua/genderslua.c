/*****************************************************************************
 *  Copyright (C) 2018 SUSE LLC
 *  Written by Christian Goll <cgoll@suse.com>
 *
 *  This file is part of Genders, a cluster configuration database.
 *  For details, see <http://www.llnl.gov/linux/genders/>.
 *
 *  Genders is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  Genders is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Genders.  If not, see <http://www.gnu.org/licenses/>.
\*****************************************************************************/
#if HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */
#include <stdlib.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <genders.h>

typedef struct {
    genders_t * handle;
    char      *name;
    char      *db_name;
 } lgenders_userdata_t;

static int lgenders_new(lua_State *L) {
	lgenders_userdata_t *dbh;
	const char *name, *db_name;
	/* check for argument vailidy */
	name = luaL_checkstring(L,1);
	db_name =luaL_checkstring(L,2); 
	if (name = NULL)
		luaL_error(L,"name cannot be empty");
	if (db_name)
		luaL_error(L,"database name could not be empty");
	/* Create the user data pushing it onto the stack. We also pre-initialize
	 * the member of the userdata in case initialization fails in some way. If
	 * that happens we want the userdata to be in a consistent state for __gc. 
	 */
	dbh = (lgenders_userdata_t *)lua_newuserdata(L, sizeof(*dbh));
	dbh->handle = NULL;
	dbh->name = NULL;
	dbh->db_name = NULL;
	/* Add the metatable to the stack. */
	luaL_getmetatable(L, "LGenders");
	/* Set the metatable on the userdata. */
	lua_setmetatable(L, -2);
	 /* Create the handle */
	dbh->name = strdup(name);
	dbh->db_name = strdup(db_name);

	return 1;
}
static int lgenders_destroy(lua_State *L) {
	lgenders_userdata_t *dbh;
	dbh = (lgenders_userdata_t *)luaL_checkudata(L, 1, "LGenders");

	if (dbh->name != NULL)
		free(dbh->name);
	dbh->name = NULL;

	if (dbh->db_name != NULL)
		free(dbh->db_name);
	dbh->db_name = NULL;

	return 0;
}


static const struct luaL_Reg lgenders_methods[] = {
	{ "__gc",        lgenders_destroy   },
	{ NULL,          NULL               },
};

static const struct luaL_Reg libgenderslua_functions[] = {
	{ "new", lgenders_new },
	{ NULL,  NULL         }
};


int luaopen_libgenderslua(lua_State *L) {
	/* Create the metatable and put it on the stack. */
	luaL_newmetatable(L, "LGenders");
	/* Duplicate the metatable on the stack (We know have 2). */
	lua_pushvalue(L, -1);
	/* Pop the first metatable off the stack and assign it to __index
	* of the second one. We set the metatable for the table to itself.
	* This is equivalent to the following in lua:
	* metatable = {}
	* metatable.__index = metatable
	*/
	lua_setfield(L, -2, "__index");

	/* Set the methods to the metatable that should be accessed via object:func */
	luaL_setfuncs(L, lgenders_methods, 0);

	/* Register the object.func functions into the table that is at the top of the
	* stack. */
	luaL_newlib(L, libgenderslua_functions);

	return 1;
}
