noinst_LIBRARIES = libmace.a

libmace_a_SOURCES = board.h board.cpp boardSquare.h boardSquare.cpp	\
piece.h piece.cpp pawn.h pawn.cpp rook.h rook.cpp knight.h knight.cpp	\
bishop.h bishop.cpp queen.cpp queen.h king.h king.cpp locus.h		\
locus.cpp search.h search.cpp mover.hpp chessBoard.cpp chessBoard.h	\
move.hpp castlingRights.hpp colour.hpp direction.hpp			\
movementTypes.hpp

bin_PROGRAMS = mace
mace_SOURCES = main.cpp
mace_LDADD = libmace.a
