# DESTDIR is usually set during make install time, not configure or
# make time, so we work around it with the --with-extension-destdir
# autoconf option.


if WITH_LUA_EXTENSIONS

luaexec_LTLIBRARIES = genders.la

genders_la_SOURCES = genderslua.c

genders_la_CFLAGS =  @LUA_INCLUDE@ -I../../libgenders/

genders_la_LDFLAGS = -module -avoid-version

genders_la_LIBADD = ../../libgenders/libgenders.la $(OTHER_FLAGS) @LUA_LIB@


endif

EXTRA_DIST = genderslua.c
