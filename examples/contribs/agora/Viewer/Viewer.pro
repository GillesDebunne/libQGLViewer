# Implementation of the game of <i>Agora</i>.

# <i>Agora</i> is a strategy game for two players. The rules are available in the help menu. The
# two players can be human or computer.

TEMPLATE = app
TARGET   = agora
CONFIG  += qt opengl warn_on release thread

HEADERS += agoraViewer.h   board.h   move.h   computerPlayer.h   undo.h   case.h
SOURCES += agoraViewer.cpp board.cpp move.cpp computerPlayer.cpp undo.cpp case.cpp main.cpp 

QT_VERSION=$$[QT_VERSION]
contains( QT_VERSION, "^4.*" ) {
  FORMS += agoraWindow.Qt4.ui computerPlayerInterface.Qt4.ui
} else {
  FORMS += agoraWindow.Qt3.ui computerPlayerInterface.Qt3.ui
}

DISTFILES *= ../Boards/*.ago

# NOT IN DISTRIBUTION BEGIN
DISTFILES *= agoraWindow.Qt3.ui computerPlayerInterface.Qt3.ui
# NOT IN DISTRIBUTION END

include( ../../../examples.pri )

