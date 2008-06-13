# Implementation of the game of <i>Agora</i>.

# <i>Agora</i> is a strategy game for two players. The code is rather complex and
# can be divided in two parts : a QGLViewer implementation for the viewer and an artificial
# intelligence that allows you to play against the computer.

# You should find the rules of the game on the web. Questions relative to the artificial
# intelligence implementation should be asked to <code>Jean-Guillaume dot Dumas at imag dot fr</code>.

# Computer player programs are expected to take three parameters: a board file name, an allowed
# time (expressed in milliseconds and which sign determines which player is to play) and the
# remaining number of moves before the game is declared over. The output should be the x,y coordinates of
# the start and end positions of the move to play.

TEMPLATE = subdirs
SUBDIRS = AI Viewer

