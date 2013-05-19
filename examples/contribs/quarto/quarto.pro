# A simulation of the game of Quarto.

# See the <code>rules.txt</code> file for explanations.

# This program was originally coded by Matthieu Richard.

TEMPLATE = app
TARGET   = quarto

HEADERS	+= glview.h jeu.h piece.h quarto.h
SOURCES	+= glview.cpp jeu.cpp piece.cpp quarto.cpp main.cpp

DISTFILES += bois.jpg rules.txt

QT_VERSION=$$[QT_VERSION]
!contains( QT_VERSION, "^3.*" ) {
  warning(This example has not been validated after Qt3)
}

include( ../../examples.pri )

