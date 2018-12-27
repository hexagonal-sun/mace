AM_DEFAULT_SOURCE_EXT = .cpp

noinst_LIBRARIES = libmace.a

libmace_a_SOURCES = board.h board.cpp moveGen.hpp moveGen.cpp locus.h	\
locus.cpp search.h search.cpp mover.hpp chessBoard.cpp chessBoard.h	\
move.hpp castlingRights.hpp colour.hpp direction.hpp			\
movementTypes.hpp uci.h uci.cpp mcts.h mcts.cpp squareState.hpp		\
squareState.cpp moveStack.hpp

bin_PROGRAMS = mace
EXTRA_PROGRAMS = perft-splitter

LDADD = libmace.a
