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
#include <lualib.h>
#include <genders.h>

typedef struct {
	/* genders_t itself is a pointer */
	genders_t handle;
	char      *db_name;
 } lgenders_userdata_t;

static int lgenders_new(lua_State *L) {
	lgenders_userdata_t *dbh;
	const char *db_name, *g_error;
	/* check for argument vailidy */
	db_name = luaL_checkstring(L,1); 
	if (db_name == NULL)
		luaL_error(L,"database name could not be empty");
	/* Create the user data pushing it onto the stack. We also pre-initialize
	 * the member of the userdata in case initialization fails in some way. If
	 * that happens we want the userdata to be in a consistent state for __gc. 
	 */
	dbh = (lgenders_userdata_t *)lua_newuserdata(L, sizeof(*dbh));
	dbh->handle = NULL;
	dbh->db_name = NULL; 
	/* Add the metatable to the stack. */
	luaL_getmetatable(L, "LGenders");
	/* Set the metatable on the userdata. */
	lua_setmetatable(L, -2);
	/* Create the handle */
	dbh->handle =  genders_handle_create(); 
	dbh->db_name = strdup(db_name);
	if(genders_load_data(dbh->handle,dbh->db_name) != 0) {
		g_error = strdup(genders_errormsg(dbh->handle));
		luaL_error(L,g_error);
	}
	return 1;
}
static int lgenders_reload(lua_State *L) {
	lgenders_userdata_t *dbh;
	const char *db_name, *g_error;
	dbh = (lgenders_userdata_t *)luaL_checkudata(L, 1, "LGenders");
	/* check for argument vailidy */
	db_name = luaL_checkstring(L,2); 
	if (db_name == NULL)
		db_name = strdup(dbh->db_name);
	if (dbh->handle != NULL)
		genders_handle_destroy(dbh->handle);
	dbh->db_name = NULL; 
	/* Create the handle */
	dbh->handle =  genders_handle_create(); 
	dbh->db_name = strdup(db_name);
	if(genders_load_data(dbh->handle,dbh->db_name) != 0) {
		g_error = strdup(genders_errormsg(dbh->handle));
		luaL_error(L,g_error);
	}
	return 0;
}
static int lgenders_destroy(lua_State *L) {
	lgenders_userdata_t *dbh;
	dbh = (lgenders_userdata_t *)luaL_checkudata(L, 1, "LGenders");
	/* pass return value to lus */
	if (dbh->handle != NULL)
		genders_handle_destroy(dbh->handle);

	if (dbh->db_name != NULL)
		free(dbh->db_name);
	dbh->db_name = NULL;
	return 0;
}

static int lgenders_getnumnodes(lua_State *L) {
	lgenders_userdata_t *dbh;
	dbh = (lgenders_userdata_t *)luaL_checkudata(L, 1, "LGenders");
	lua_pushinteger(L,genders_getnumnodes(dbh->handle));
	return 1;
}

static int lgenders_getnumattrs(lua_State *L) {
	lgenders_userdata_t *dbh;
	dbh = (lgenders_userdata_t *)luaL_checkudata(L, 1, "LGenders");
	lua_pushinteger(L,genders_getnumattrs(dbh->handle));
	return 1;
}

static int lgenders_getnodes(lua_State *L) {
	char** nodelist;
	const char *g_error, *attr, *val;
	int size, nr_nodes, i, nr_args = 0;
	lgenders_userdata_t *dbh;
	dbh = (lgenders_userdata_t *)luaL_checkudata(L, 1, "LGenders");
	/* create space for the genders stuff */
	size = genders_nodelist_create(dbh->handle,&nodelist);
	if(size == -1) {
		g_error = strdup(genders_errormsg(dbh->handle));
		luaL_error(L,g_error);
	}
	attr = NULL; val = NULL;
	/* check for attr and val */
	nr_args = lua_gettop(L);
	if(nr_args > 1)
		attr = luaL_checkstring(L,2); 
	if(nr_args > 2)
		val = luaL_checkstring(L,3); 
	if(nr_args > 3)
		luaL_error(L,"getnodes accepts none,one or two arguments");
	nr_nodes = genders_getnodes(dbh->handle,nodelist,size,attr,val);
	lua_newtable(L);
	for(i = 0; i < nr_nodes; i++) {
		lua_pushstring(L,nodelist[i]);
		lua_rawseti(L,-2,i+1);
	}
	/* destroy list of nodes */
	if(genders_nodelist_destroy(dbh->handle,nodelist) == -1) {
		g_error = strdup(genders_errormsg(dbh->handle));
		luaL_error(L,g_error);
	}
	return 1;
}

static int lgenders_query(lua_State *L) {
	char** nodelist;
	const char *g_error, *query;
	int size, nr_nodes, i, nr_args = 0;
	lgenders_userdata_t *dbh;
	dbh = (lgenders_userdata_t *)luaL_checkudata(L, 1, "LGenders");
	/* create space for the genders stuff */
	size = genders_nodelist_create(dbh->handle,&nodelist);
	if(size == -1) {
		g_error = strdup(genders_errormsg(dbh->handle));
		luaL_error(L,g_error);
	}
	nr_args = lua_gettop(L);
	if(nr_args == 2)
		query = luaL_checkstring(L,2); 
	else
		luaL_error(L,"query must be called with one argument");
	nr_nodes = genders_query(dbh->handle,nodelist,size,query);
	lua_newtable(L);
	for(i = 0; i < nr_nodes; i++) {
		lua_pushstring(L,nodelist[i]);
		lua_rawseti(L,-2,i+1);
	}
	/* destroy list of nodes */
	if(genders_nodelist_destroy(dbh->handle,nodelist) == -1) {
		g_error = strdup(genders_errormsg(dbh->handle));
		luaL_error(L,g_error);
	}
	return 1;
}
static int lgenders_getattr(lua_State *L) {
	char **attr_list, **val_list;
	const char *node, *g_error;
	int ret_code, nr_args, size_attr, i = 0;
	lgenders_userdata_t *dbh;
	dbh = (lgenders_userdata_t *)luaL_checkudata(L, 1, "LGenders");
	nr_args = lua_gettop(L);
	if(nr_args == 2)
		node = luaL_checkstring(L,2); 
	else
		luaL_error(L,"query must be called with one argument");
	/* create space for the lists */
	ret_code = genders_attrlist_create(dbh->handle,&attr_list);
	if(ret_code == -1) {
		g_error = strdup(genders_errormsg(dbh->handle));
		luaL_error(L,g_error);
	}
	ret_code = genders_vallist_create(dbh->handle,&val_list);
	if(ret_code == -1) {
		g_error = strdup(genders_errormsg(dbh->handle));
		luaL_error(L,g_error);
	}
	/* hopefully size (which is in ret_code) of attr_list and val_list are the same */
	size_attr = genders_getattr(dbh->handle,attr_list,val_list,ret_code,node);
	if(size_attr == -1) {
		g_error = strdup(genders_errormsg(dbh->handle));
		luaL_error(L,g_error);
	}
	lua_newtable(L);
	for(i = 0; i < size_attr; i++) {
		printf("attr: %s <-> val %s\n",attr_list[i],val_list[i]);
		lua_pushstring(L, attr_list[i]);
		lua_pushstring(L, val_list[i]);
		lua_settable(L, -3);
	}
	/* destroy list of nodes */
	if(genders_vallist_destroy(dbh->handle,val_list) == -1) {
		g_error = strdup(genders_errormsg(dbh->handle));
		luaL_error(L,g_error);
	}
	if(genders_attrlist_destroy(dbh->handle,attr_list) == -1) {
		g_error = strdup(genders_errormsg(dbh->handle));
		luaL_error(L,g_error);
	}
	return 1;
}

static int lgenders_isnode(lua_State *L) {
	const char *node, *g_error;
	int ret_code, nr_args;
	lgenders_userdata_t *dbh;
	dbh = (lgenders_userdata_t *)luaL_checkudata(L, 1, "LGenders");
	nr_args = lua_gettop(L);
	if(nr_args == 2)
		node = luaL_checkstring(L,2); 
	else
		luaL_error(L,"is_node must be called with one argument");
	ret_code = genders_isnode(dbh->handle,node);
	if(ret_code == -1) {
		g_error = strdup(genders_errormsg(dbh->handle));
		luaL_error(L,g_error);
	}
	lua_pushboolean(L,ret_code);
	return 1;
}

static int lgenders_isattr(lua_State *L) {
	const char *attr, *g_error;
	int ret_code, nr_args;
	lgenders_userdata_t *dbh;
	dbh = (lgenders_userdata_t *)luaL_checkudata(L, 1, "LGenders");
	nr_args = lua_gettop(L);
	if(nr_args == 2)
		attr = luaL_checkstring(L,2); 
	else
		luaL_error(L,"is_attr must be called with one argument");
	ret_code = genders_isattr(dbh->handle,attr);
	if(ret_code == -1) {
		g_error = strdup(genders_errormsg(dbh->handle));
		luaL_error(L,g_error);
	}
	lua_pushboolean(L,ret_code);
	return 1;
}

static int lgenders_isattrval(lua_State *L) {
	const char *attr, *val, *g_error;
	int ret_code, nr_args;
	lgenders_userdata_t *dbh;
	dbh = (lgenders_userdata_t *)luaL_checkudata(L, 1, "LGenders");
	nr_args = lua_gettop(L);
	if(nr_args == 3) {
		attr = luaL_checkstring(L,2); 
		val = luaL_checkstring(L,3); 
	}
	else
		luaL_error(L,"is_attrval must be called with two arguments");
	ret_code = genders_isattrval(dbh->handle,attr,val);
	if(ret_code == -1) {
		g_error = strdup(genders_errormsg(dbh->handle));
		luaL_error(L,g_error);
	}
	lua_pushboolean(L,ret_code);
	return 1;
}

static const struct luaL_Reg genders_methods[] = {
	{"getnumattrs",lgenders_getnumattrs},
	{"getnumnodes",lgenders_getnumnodes},
	{"getnodes",lgenders_getnodes},
	{"getattr",lgenders_getattr},
	{"query",lgenders_query},
	{"isnode",lgenders_isnode},
	{"isattr",lgenders_isattr},
	{"isattrval",lgenders_isattrval},
	{"reload",lgenders_reload},
	{"__gc",lgenders_destroy},
	{NULL,NULL},
};

static const struct luaL_Reg genders_functions[] = {
	{ "new", lgenders_new },
	{ NULL,  NULL}
};


int luaopen_genders(lua_State *L) {
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
	luaL_setfuncs(L, genders_methods, 0);

	/* Register the object.func functions into the table that is at the top of the
	* stack. */
	luaL_newlib(L, genders_functions);

	return 1;
}
