#include <fstream>
#include <stdio.h>
#include "board.h"

int main (int, char ** argv)
{    
  Board board;
  
  std::ifstream file(argv[1]);
  file >> board;
  file.close();

  std::cout << board.bestMoveNumberOfNewPieces();

  return 0;
}
