#ifndef BOARD_H
#define BOARD_H
#include "../Graph_lib/Graph.h"
#include "../Graph_lib/Simple_window.h"
#include "cell.h"

using namespace Graph_lib;
using namespace std;

class Board : public Graph_lib::Window
{
public:
  Board();
  void await();
  bool game_over() {return end;}

protected:
  void draw();
private:
  void up_set();
  void down_set();
  void left_set();
  void right_set();
  void random_cell();
  void restart();

  //пошло отсюда...
  Button left_but;
  static void cb_left (Address , Address pw);

  Button right_but;
  static void cb_right (Address , Address pw);

  Button down_but;
  static void cb_down (Address , Address pw);

  Button up_but;
  static void cb_up (Address , Address pw);

  Button restart_but;
  static void cb_restart (Address , Address pw);

  bool left_pd{false}; //нажатие кнопки
  bool right_pd{false};
  bool down_pd{false};
  bool up_pd{false};
  bool restart_pd{false};


  bool end{false};
  int score = 0;
  std::array<Vector_ref<Cell>, 4> table;

  Vector_ref<Graph_lib::Rectangle> rect;
  Vector_ref<Text> box;
};

#endif // BOARD_H
