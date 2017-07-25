#include "../Viewer/board.h"
#include <fstream>
#include <stdio.h>

int main(int argc, char **argv) {
  Board board;

  if (argc != 3) {
    std::cout << "This AI blobwar player requires 2 parameters. It should be "
                 "called by the main blobWar viewer application."
              << std::endl;
    exit(1);
  }

  std::ifstream file(argv[1]);
  file >> board;
  file.close();

  std::cout << board.bestMoveNumberOfNewPieces();

  return 0;
}
