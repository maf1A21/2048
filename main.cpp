#include "board.h"

int main()
{
  Board game;
  while (!game.game_over())
    game.await(); // waiting for any moves

  game.await();

  return 0;
}
