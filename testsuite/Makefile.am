check_PROGRAMS = perft-tester zobrist-tester piececounts
AM_DEFAULT_SOURCE_EXT = .cpp
AM_CXXFLAGS = -I$(top_srcdir)/src
LDADD = $(top_builddir)/src/libmace.a

TESTS = starting-position.perft position2.perft position3.perft		\
    position4.perft position5.perft position6.perft zobrist-tester	\
    piececounts

TEST_EXTENSIONS = .perft
PERFT_LOG_COMPILER = ./perft-tester
EXTRA_DIST = $(TESTS)
