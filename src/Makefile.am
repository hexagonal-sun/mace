AM_DEFAULT_SOURCE_EXT = .cpp

noinst_LIBRARIES = libmace.a

libmace_a_SOURCES = board.h board.cpp moveGen.h moveGen.cpp locus.h	\
locus.cpp search.h search.cpp mover.hpp chessBoard.cpp chessBoard.h	\
move.hpp castlingRights.hpp colour.hpp direction.hpp uci.h uci.cpp	\
squareState.hpp squareState.cpp moveStack.hpp evaluate.cpp evaluate.h	\
moveTypes/diagonal.hpp moveTypes/knight.hpp moveTypes/orthogonal.hpp	\
moveTypes/orthodiagonal.hpp pieceType.hpp zobrist.h zobrist.cpp		\
transpositionTable.cpp transpositionTable.h

bin_PROGRAMS = mace
EXTRA_PROGRAMS = perft-splitter

LDADD = libmace.a
CXXFLAGS += $(PTHREAD_CFLAGS)
LDFLAGS += $(PTHREAD_LDFLAGS)
