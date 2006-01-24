# todo

TEMPLATE = app
TARGET   = frustumCulling
CONFIG  += qt opengl warn_on release thread

HEADERS  = frustumCulling.h cullingCamera.h box.h
SOURCES  = frustumCulling.cpp cullingCamera.cpp box.cpp main.cpp

include( ../examples.pri )
