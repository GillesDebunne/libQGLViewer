#include "../Viewer/board.h"
#include <fstream>
#include <stdio.h>

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cout << "This AI agora player requires 3 parameters. It should be "
                 "called by the main agora viewer application."
              << std::endl;
    exit(1);
  }

  Board board;

  std::ifstream file(argv[1]);
  file >> board;
  file.close();

  std::cout << board.randomMove(QString(argv[2]).toInt() >= 0) << std::endl;

  return 0;
}
