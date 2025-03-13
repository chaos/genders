
AC_DEFUN([AC_LUA_EXTENSIONS],
[
  AC_MSG_CHECKING(for --with-lua-extensions)
  AC_ARG_WITH(lua-extensions,
     AC_HELP_STRING([--with-lua-extensions=], 
                    [enable or disable lua extensions build]),
     [ case "$withval" in
     yes)
         ac_with_lua_extensions=yes
         ;;
     no)
         ac_with_lua_extensions=no
         ;;
     *)
         ac_with_lua_extensions=yes
         ;;
     esac ]
  )
  AC_MSG_RESULT(${ac_with_lua_extensions=no})
])
