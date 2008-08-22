TEMPLATE = app
TARGET   = agoraAI 
CONFIG  += qt warn_on release thread console

QT += xml opengl

INCLUDEPATH *= ../Viewer

HEADERS = ../Viewer/board.h ../Viewer/move.h ../Viewer/undo.h ../Viewer/case.h
SOURCES = ai.cpp ../Viewer/board.cpp ../Viewer/move.cpp ../Viewer/undo.cpp ../Viewer/case.cpp

include( ../../../examples.pri )
