TEMPLATE = app
TARGET   = blobWarAI 

INCLUDEPATH *= ../Viewer

HEADERS = ../Viewer/board.h ../Viewer/move.h ../Viewer/undo.h
SOURCES = ai.cpp ../Viewer/board.cpp ../Viewer/move.cpp ../Viewer/undo.cpp

include( ../../../examples.pri )
