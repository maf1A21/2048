#include "board.h"
#include <ctime>
#include "iostream"
#include "../Graph_lib/Graph.h"
#include <cstdlib>

Board::Board()
  :Window{Point{0,0} , 510 ,500 , "2048"} ,
    left_but{Point{400,400} , 35 , 35 , "left" , cb_left},
    right_but{Point{470,400} , 35 , 35 , "right" , cb_right},
    down_but{Point{435,435} , 35 , 35 , "down" , cb_down},
    up_but{Point{435,365} , 35 , 35 , "up" , cb_up},
    restart_but{Point{425,110} , 50 , 35 , "restart" , cb_restart},
    left_pd{false} , right_pd{false} ,
    down_pd{false} , up_pd{false}  ,restart_pd{false}
{
  for (int i{0} ; i<4 ; ++i)
  {
    for (int j{0} ; j<4 ; ++j)
    {
      table[i].push_back(new Cell(0));
    }
  }
  random_cell();
  random_cell();

  attach(right_but);
  attach(left_but);
  attach(down_but);
  attach(up_but);
  attach(restart_but);
}


void Board::cb_up(Address, Address pw)
{
  reference_to<Board>(pw).up_set();
}

void Board::cb_down(Address, Address pw)
{
  reference_to<Board>(pw).down_set();
}

void Board::cb_left (Address , Address pw)
{
  reference_to<Board>(pw).left_set();
}

void Board::cb_right (Address , Address pw)
{
  reference_to<Board>(pw).right_set();
}

void Board::cb_restart(Address, Address pw)
{
  reference_to<Board>(pw).restart();
}


void Board::await()
{
  while (!right_pd && !left_pd && !up_pd &&
         !restart_pd && !down_pd && Fl::wait())
    ;

  right_pd = false;
  left_pd = false;
  up_pd = false;
  down_pd = false;
  restart_pd = false;

  Fl::redraw();
}

//game mechanics part

void Board::random_cell()
{
  std::vector<std::vector<int>> free_cell;

  for(int i {0} ; i<4 ; ++i)
  {
    for(int j {0} ; j<4 ; ++j)
    {
      if (table[i][j].get_bin()==0)
        free_cell.push_back(std::vector<int>{i,j});
    }
  }

  if (free_cell.size()==0)
  {
    end = true;
    return;
  }


  int a = free_cell.size();

  if (a-1 == 0)
  {
    int i = free_cell[0][0];
    int j = free_cell[0][1];
    table[i][j] = Cell{1};
    return;
  }


  srand((unsigned int)time(NULL));
  a = abs(rand() % a);
  table[free_cell[a][0]][free_cell[a][1]]=Cell{1};
}


void Board::down_set()
{
  for (int i{0} ; i<4 ; i++)
  {
    for(int l{3} ; l>=2 ; --l)
    {
      for (int j{l} ; j>=1 ; --j)
      {
        if(table[j][i].get_bin()==0)
          for (int h{0} ; h<4 ; ++h)
          {
            for(int J{j} ; J>=1 ; --J)
              if(table[J][i].get_bin()==0 && J>=1)
                std::swap(table[J-1][i],table[J][i]);
          }
      }

      for (int j{l} ; j>=1 ; --j)
      {
        if (table[j][i].get_bin()==table[j-1][i].get_bin() && table[j][i].get_bin()!=0)
        {
          score += (pow(2, table[j][i].get_bin()));
          table[j][i].plus_pow();
          table[j-1][i]=Cell(0);
        }
      }
    }
  }
  random_cell();
  down_pd = true;
}


void Board::up_set()
{
  for (int i{0} ; i<4 ; i++)
  {
    for(int j{0} ; j<=2 ; ++j)
    {
      if(table[j][i].get_bin()==0)
      {
        for(int h{0} ; h<4 ; ++h)
        {
          for(int J{j} ; J<=2 ; ++J)
          {
            if(table[J][i].get_bin()==0 && J<=2)
              std::swap(table[J+1][i],table[J][i]);
          }
        }
      }
    }

    for (int j{0} ; j<3 ; ++j)
    {
      if (table[j][i].get_bin()==table[j+1][i].get_bin() && table[j][i].get_bin()!=0)
      {
        score += (pow(2, table[j][i].get_bin()));
        table[j][i].plus_pow();
        table[j+1][i]=Cell(0);
      }
    }
    for(int j{0} ; j<=2 ; ++j)
      if(table[j][i].get_bin()==0)
      {
        for(int h{0} ; h<4 ; ++h)
        {
          for(int J{j} ; J<=2 ; ++J)
          {
            if(table[J][i].get_bin()==0 && J<=2)
              std::swap(table[J+1][i],table[J][i]);
          }
        }
      }
  }
  random_cell();
  up_pd = true;
}


void Board::right_set()
{
  for (int i{0} ; i<4 ; i++)
  {
    for(int l{3} ; l>=2 ; --l) //хз l>=2 или l>=1
    {
      for (int j{l} ; j>=1 ; --j)//сдвиг не 0 вправо до конца
      {
        if(table[i][j].get_bin()==0)
          for (int h{0} ; h<4 ; ++h)
          {
            for(int J{j} ; J>=1 ; --J)
              if(table[i][J].get_bin()==0 && J>=1)
                std::swap(table[i][J-1],table[i][J]);
          }
      }

      for (int j{l} ; j>=1 ; --j)//слияние вправо
      {
        if (table[i][j].get_bin()==table[i][j-1].get_bin() && table[i][j].get_bin()!=0)
        {
          table[i][j].plus_pow();
          score += (pow(2, table[i][j].get_bin()))/2;
          table[i][j-1]=Cell(0);
        }
      }
    }
  }
  random_cell();
  right_pd = true;
}


void Board::left_set()
{
  for (int i{0} ; i<4 ; i++)
  {
    for(int j{0} ; j<=2 ; ++j)
    {
      if(table[i][j].get_bin()==0)
      {
        for(int h{0} ; h<4 ; ++h)
        {
          for(int J{j} ; J<=2 ; ++J)
          {
            if(table[i][J].get_bin()==0 && J<=2)
              std::swap(table[i][J+1],table[i][J]);
          }
        }
      }
    }

    for (int j{0} ; j<3 ; ++j)
    {
      if (table[i][j].get_bin()==table[i][j+1].get_bin() && table[i][j].get_bin()!=0)
      {
        score += (pow(2, table[i][j].get_bin()));
        table[i][j].plus_pow();
        table[i][j+1]=Cell(0);
      }
    }
    for(int j{0} ; j<=2 ; ++j)
      if(table[i][j].get_bin()==0)
      {
        for(int h{0} ; h<4 ; ++h)
        {
          for(int J{j} ; J<=2 ; ++J)
          {
            if(table[i][J].get_bin()==0 && J<=2)
              std::swap(table[i][J+1],table[i][J]);
          }
        }
      }
  }
  random_cell();
  left_pd = true;
}


void Board::draw() //здесь живёт зло, крадущее оперативку
{
  Window::draw();

  for (int i{0} ; i<4 ; i++)
  {
    for (int j{0} ; j<4 ; j++)
    {
      rect.push_back(new Graph_lib::Rectangle{Point{j*100,i*100},100,100});
      rect[rect.size()-1].set_fill_color(table[i][j].get_color());
      rect[rect.size()-1].draw();

      if (table[i][j].get_bin()>0)
      {
        std::string s = std::to_string((int)pow(2,table[i][j].get_bin()));
        box.push_back(new Graph_lib::Text{Point{j*100+40, i*100+60} , s });
        box[box.size()-1].set_font_size(40);
        box[box.size()-1].draw();
      }
    }
  }

  Graph_lib::Text scr {Point {410, 13}, "Score: "};
  scr.set_font_size(15);
  scr.draw();

  std::string  s = std::to_string((int) score);
  Graph_lib::Text a {Point {460, 13}, s};
  a.set_font_size(15);
  a.draw();


  if (end)
  {
    std::string  c = std::to_string((int) score);
    string over_text = "Game is over! Your score: " + c;
    Graph_lib::Rectangle over{Point {50, 150}, 400, 100};
    over.set_fill_color(Color::white);
    over.draw();
    Graph_lib::Text gg {Point {90, 200}, over_text};
    gg.set_font_size(25);
    gg.draw();
  }
}


void Board::restart()
{
  for(int i{0} ; i<4 ; ++i)
    for(int j{0} ; j<4 ; ++j)
    {
      table[i][j] = Cell{0};
    }
  score = 0;

  random_cell();
  random_cell();

  restart_pd = true;
}

