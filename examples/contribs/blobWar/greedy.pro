TEMPLATE = app
TARGET   = greedy 
CONFIG  += qt warn_on release thread

QT += xml opengl

HEADERS = board.h move.h undo.h
SOURCES = greedy.cpp board.cpp move.cpp undo.cpp

OBJECTS_DIR = .obj

