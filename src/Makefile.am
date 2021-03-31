AM_CPPFLAGS = $(fuse_CFLAGS)

bin_PROGRAMS = ntapfuse

ntapfuse_SOURCES = ntapfuse.c ntapfuse_ops.c ntapfuse_ops.h business_logic.c business_logic.h database.c database.h htable.h htable.c htable_record_cache.h htable_record_cache.c htable_mutex.h htable_mutex.c common.h common.c

ntapfuse_LDADD = $(fuse_LIBS)
