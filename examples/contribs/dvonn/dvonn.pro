# Implementation of the game of <i>Dvonn</i>.

# <i>Dvonn</i> is a very good strategy game for two players. See the Help menu for complete rules.
# Consider buying the game if you liked it. It is quite cheap and really nicer than this version.

# See the <a href="http://www.gipf.com/dvonn/index.html">GIPF project web page</a> for details.

# This example was created by Xavier Decoret.

TEMPLATE = app
TARGET   = dvonn
CONFIG  += qt opengl warn_on release thread

HEADERS += board.h   game.h   drawer.h   dvonnviewer.h   dvonnwindowimpl.h
SOURCES += board.cpp game.cpp drawer.cpp dvonnviewer.cpp dvonnwindowimpl.cpp main.cpp

FORMS += dvonnwindow.ui

# Optimization flags
QMAKE_CXXFLAGS_RELEASE *= -O7 -funroll-all-loops -felide-constructors -fstrict-aliasing -frerun-loop-opt \
			  -fexpensive-optimizations -fforce-addr -fforce-mem -fstrength-reduce

# "make dist" additionnal files
DISTFILES += images/*.png rules/*.png rules/rules.html

include( ../examples.pri )
