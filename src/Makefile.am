AM_DEFAULT_SOURCE_EXT = .cpp

noinst_LIBRARIES = libmace.a

libmace_a_SOURCES = board.h board.cpp boardSquare.h boardSquare.cpp	\
piece.h piece.cpp pawn.h pawn.cpp rook.h rook.cpp knight.h knight.cpp	\
bishop.h bishop.cpp queen.cpp queen.h king.h king.cpp locus.h		\
locus.cpp search.h search.cpp mover.hpp chessBoard.cpp chessBoard.h	\
move.hpp castlingRights.hpp colour.hpp direction.hpp			\
movementTypes.hpp uci.h uci.cpp mcts.h mcts.cpp

bin_PROGRAMS = mace
EXTRA_PROGRAMS = perft-splitter

LDADD = libmace.a
