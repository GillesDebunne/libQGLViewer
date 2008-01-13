# Implementation of the game of <i>BlobWar</i>.

# <i>BlobWar</i> is a strategy game for two players. The rules are available in the help menu. The
# two players can be human or computer.

# Computer player programs are expected to take two parameters: a board file name and an allowed
# time (which sign determines which player is to play). The output should be the x,y coordinates of
# the start and end positions of the move to play.

# A naive greedy AI program is provided in the AI directory.

TEMPLATE = subdirs
SUBDIRS = AI Viewer

