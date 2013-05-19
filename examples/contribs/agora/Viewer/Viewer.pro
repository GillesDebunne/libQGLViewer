# Implementation of the game of <i>Agora</i>.

# <i>Agora</i> is a strategy game for two players. The rules are available in the help menu. The
# two players can be human or computer.

TEMPLATE = app
TARGET   = agora

HEADERS += agoraViewer.h   board.h   move.h   computerPlayer.h   undo.h   case.h
SOURCES += agoraViewer.cpp board.cpp move.cpp computerPlayer.cpp undo.cpp case.cpp main.cpp

QT_VERSION=$$[QT_VERSION]
contains( QT_VERSION, "^3.*" ) {
  FORMS += agoraWindow.Qt3.ui computerPlayerInterface.Qt3.ui
} else {
  FORMS += agoraWindow.ui computerPlayerInterface.ui
}

DISTFILES *= ../AgoraBoards/*.ago

include( ../../../examples.pri )

