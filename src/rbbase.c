#include "rbclips.h"
#include "rbbase.h"
#include "rbrouter.h"
#include "rbenvironment.h"
#include "rbexception.h"
#include "clips/clips.h"

/* Definitions */
VALUE cl_cBase;

/**
 * Run given CLIPS command and return it's result
 */
VALUE cl_base_insert_command(VALUE self, VALUE cmd)
{
  // Param check
  if(TYPE(cmd) != T_STRING)
  {
    rb_raise(cl_eArgError, "Base::insert_command accept only String as parametr!");
    return Qfalse;
  }

  // Execution needs string ended by new line character and user may not
  // add it. It will work with two new line characters anyway, so I simply
  // append \n and I don't check whether the \n is alrady at the end
  VALUE c = rb_str_cat(cmd, "\n", 1);

  void *env = GetCurrentEnvironment();

  FlushCommandString(env);
  AppendCommandString(env, StringValueCStr(c));

  // Executing command
  if( ! ExecuteIfCommandComplete(env) || cl_router_werror())
  {
    char *msg = STR2CSTR(cl_router_get_content_d());

    if(strlen(msg) == 0)
    {
      rb_raise(cl_eException, "Unknown error, probably the inserted command is not valid CLIPS command.");
    } else {
      rb_raise(cl_eException, "%s",  msg );
    }

    return Qfalse;
  }

  // Returning output (if any)
  return cl_router_get_content_d();
}
