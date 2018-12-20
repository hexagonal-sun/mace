check_PROGRAMS = perft-tester
AM_DEFAULT_SOURCE_EXT = .cpp
AM_CXXFLAGS = -I$(top_srcdir)/src
LDADD = $(top_builddir)/src/libmace.a

TESTS = starting-position.perft position2.perft
TEST_EXTENSIONS = .perft
PERFT_LOG_COMPILER = ./perft-tester
