TEMPLATE = app
TARGET   = greedy 
CONFIG  += warn_on release thread

HEADERS = board.h move.h undo.h
SOURCES = greedy.cpp board.cpp move.cpp undo.cpp

OBJECTS_DIR = .obj

