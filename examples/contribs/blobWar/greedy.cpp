#include <fstream>
#include <stdio.h>
#include "board.h"

int main (int, char ** argv)
{    
  Board board;
  
  std::ifstream file(argv[1]);
  file >> board;
  file.close();

  bool bluePlays = atof(argv[2]) > 0;

  std::cout << board.bestMoveNumberOfNewPieces(bluePlays);

  return 0;
}
