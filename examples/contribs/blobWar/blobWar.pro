# Implementation of the game of <i>BlobWar</i>.

# <i>BlobWar</i> is a strategy game for two players.
# ...


TEMPLATE = app
TARGET   = blobWar
CONFIG  += qt opengl warn_on release thread
VERSION  = 1.2

HEADERS += blobWarViewer.h board.h move.h computerPlayer.h undo.h
SOURCES += main.cpp blobWarViewer.cpp board.cpp boardDraw.cpp move.cpp computerPlayer.cpp undo.cpp

QT_VERSION=$$[QT_VERSION]
contains( QT_VERSION, "^4.*" ) {
  FORMS += blobWarWindow.Qt4.ui computerPlayerInterface.Qt4.ui
} else {
  FORMS += blobWarWindow.Qt3.ui computerPlayerInterface.Qt3.ui
}

DISTFILES *= Boards/*.bwb greedy.cpp greedy.pro

# NOT IN DISTRIBUTION BEGIN
DISTFILES *= blobWarWindow.Qt4.ui computerPlayerInterface.Qt4.ui
# NOT IN DISTRIBUTION END

include( ../../examples.pri )
QT += xml opengl



# JG :
# ! Attention, ne pas mettre le "QGLViewer" final car les include sont de la forme "QGLViewer/toto.h"
#INCLUDEPATH *= /path/to/QGLViewerDir
#LIBS *= -L/le/path/vers/le.so -lQGLViewer
