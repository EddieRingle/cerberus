/*
** Lua binding: Entity
** Generated automatically by tolua++-1.0.92 on Sun Dec 25 21:11:25 2011.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_Entity_open (lua_State* tolua_S);

#include "../entity.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"entity");
}

/* get function: id of class  entity */
#ifndef TOLUA_DISABLE_tolua_get_entity_id
static int tolua_get_entity_id(lua_State* tolua_S)
{
  entity* self = (entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'id'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->id);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: id of class  entity */
#ifndef TOLUA_DISABLE_tolua_set_entity_id
static int tolua_set_entity_id(lua_State* tolua_S)
{
  entity* self = (entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'id'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->id = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: name of class  entity */
#ifndef TOLUA_DISABLE_tolua_get_entity_name
static int tolua_get_entity_name(lua_State* tolua_S)
{
  entity* self = (entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'name'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_Entity_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"entity","entity","",NULL);
  tolua_beginmodule(tolua_S,"entity");
   tolua_variable(tolua_S,"id",tolua_get_entity_id,tolua_set_entity_id);
   tolua_variable(tolua_S,"name",tolua_get_entity_name,NULL);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_Entity (lua_State* tolua_S) {
 return tolua_Entity_open(tolua_S);
};
#endif

