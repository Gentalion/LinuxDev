TESTS = test 
check_PROGRAMS = test
CFLAGS = @CHECK_CFLAGS@ -I../src
LDADD = $(top_builddir)/src/libbuf.la @CHECK_LIBS@

.ts.c:
	checkmk $< > $@