#include "cell.h"

int Cell::Set_color()
{
  switch(b_pow)
  {
  case 0:
    return 17;
  case 1:
    return 47;
  case 2:
    return 19;
  case 3:
    return 122;
  case 4:
    return 131;
  case 5:
    return 89;
  case 6:
    return 2;
  case 7:
    return 94;
  case 8:
    return 95;
  case 9:
    return 126;
  case 10:
    return 174;
  case 11:
    return 134;
  default:
    return 63;
  }
}


Cell::Cell(int bin)
{
  b_pow=bin;
}
