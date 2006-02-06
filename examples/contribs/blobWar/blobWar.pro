# Implementation of the game of <i>BlobWar</i>.

# <i>BlobWar</i> is a strategy game for two players. The rules are available in the help menu. The
# two players can be human or computer.

# Computer player programs are expected to take two parameters: a board file name and an allowed
# time (which sign determines which player is to play). The output should be the x,y coordinates of
# the start and end positions of the move to play.

# A naive greedy program is provided. Compile it using the <code>qmake greedy.pro</code> file (type
# <code>qmake greedy.pro ; make</code> on Unix architectures).

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

# JG :
QT += xml opengl
# ! Attention, ne pas mettre le "QGLViewer" final car les include sont de la forme "QGLViewer/toto.h"
#INCLUDEPATH *= /path/to/QGLViewerDir
#LIBS *= -L/le/path/vers/le.so -lQGLViewer

# NOT IN DISTRIBUTION END

include( ../../examples.pri )

