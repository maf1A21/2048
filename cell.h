#ifndef CELL_H
#define CELL_H

class Cell
{
public:
  Cell(int bin);

  void plus_pow()
  {
    b_pow++;
  }

  int get_bin() const {return b_pow;}
  int get_color(){return Set_color();}

private:
  int Set_color();
  int b_pow;
};

#endif // CELL_H
