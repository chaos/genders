# DESTDIR is usually set during make install time, not configure or
# make time, so we work around it with the --with-extension-destdir
# autoconf option.


if WITH_LUA_EXTENSIONS
luaexec_LTLIBRARIES       = libgenders.la

libgenders_la_CFLAGS = @LUA_INCLUDE@

libgenders_la_SOURCES = genderslua.c

libgenders_la_LIBADD = ../../libgenders/libgenders.la

libgenders_la_LDFLAGS = -version-info @LUA_LIB@ @LIBGENDERSLUA_VERSION_INFO@ $(OTHER_FLAGS)

#../../libgenders/libgenderslua.la: force-dependency-check
#	@cd `dirname $@` && make `basename $@`
#force-dependency-check:

endif
