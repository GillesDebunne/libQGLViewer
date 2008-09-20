# A simulation of the game of Quarto.

# See the <code>rules.txt</code> file for explanations.

# This program was originally coded by Matthieu Richard.

TEMPLATE = app
TARGET   = quarto
CONFIG  += qt opengl warn_on release thread

HEADERS	+= glview.h jeu.h piece.h quarto.h
SOURCES	+= glview.cpp jeu.cpp piece.cpp quarto.cpp main.cpp

DISTFILES += bois.jpg rules.txt

QT_VERSION=$$[QT_VERSION]
contains( QT_VERSION, "^4.*" ) {
  warning(This example has not yet been ported to Qt4)
}

include( ../../examples.pri )

