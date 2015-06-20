# Implementation of the game of <i>Dvonn</i>.

# <i>Dvonn</i> is a very good strategy game for two players. See the Help menu for complete rules.
# Consider buying the game if you liked it. It is quite cheap and really nicer than this version.

# See the <a href="http://www.gipf.com/dvonn/index.html">GIPF project web page</a> for details.

# This example was created by Xavier Decoret.

TEMPLATE = app
TARGET   = dvonn

HEADERS += board.h   game.h   drawer.h   dvonnviewer.h   dvonnwindowimpl.h
SOURCES += board.cpp game.cpp drawer.cpp dvonnviewer.cpp dvonnwindowimpl.cpp main.cpp

# "make dist" additionnal files
DISTFILES += images/*.png rules/*.png rules/rules.html

QT_VERSION=$$[QT_VERSION]
contains( QT_VERSION, "^3.*" ) {
  FORMS += dvonnwindow.Qt3.ui
} else {
  FORMS += dvonnwindow.ui
}

# Since we use gluBuild2DMipmaps
!macx|darwin-g++ {
    QMAKE_LIBS_OPENGL *= -lGLU
}

include( ../../examples.pri )

