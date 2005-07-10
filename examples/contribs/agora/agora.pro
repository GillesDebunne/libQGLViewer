# Implementation of the game of <i>Agora</i>.

# <i>Agora</i> is a strategy game for two players. The code is rather complex and
# can be divided in two parts : a QGLViewer implementation for the viewer and an artificial
# intelligence that allows you to play against the computer.

# You should find the rules of the game on the web. Questions relative to the artificial
# intelligence implementation should be asked to <code>Jean-Guillaume dot Dumas at imag dot fr</code>.

TEMPLATE = app
TARGET   = agora
CONFIG  += qt opengl warn_on release thread

# Input
HEADERS += agora_alphabeta.inl  agora_coupspossibles.inl  agora_init.inl  agora_jouer.inl   agoraViewer.h
HEADERS += agora_class.h        agora_evaluation.inl      agora_io.h      container_stream.h agora_container.h
HEADERS += agora_coup.h         agora.h                   agora_io.inl    agora_types.h     givtimer.h
SOURCES += main.cpp givtimer.C agoraViewer.cpp

FORMS += agoraWindow.ui

# "make dist" additionnal files
DISTFILES += wood.png

QMAKE_CXXFLAGS_RELEASE *= -O7 -funroll-all-loops -felide-constructors -fstrict-aliasing -frerun-loop-opt \
			  -fexpensive-optimizations -fforce-addr -fforce-mem -fstrength-reduce

# JG's paths
#INCLUDEPATH *=/home/soft/libQGLViewer-1.3.5/include
#LIBS *= -L/home/soft/libQGLViewer-1.3.5/lib -lQGLViewer

include( ../examples.pri )
