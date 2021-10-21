
Для начала опишем суть и правила игры.

1) В каждом раунде появляется клетка номинала «2»

2) Нажатием стрелки игрок может скинуть все плитки игрового поля в одну из 4 сторон. 

3) Если при сбрасывании две плитки одного номинала «налетают» одна на другую, то они слипаются в одну, номинал которой равен сумме соединившихся плиток. После каждого хода на свободной секции поля появляется новая плитка номиналом «2» . Если при нажатии кнопки местоположение плиток или их номинал не изменится, то ход не совершается.

4) Если в одной строчке или в одном столбце находится более двух плиток одного номинала, то при сбрасывании они начинают слипаться с той стороны, в которую были направлены. Например, находящиеся в одной строке плитки (4, 4, 4) после хода влево они превратятся в (8, 4), а после хода вправо — в (4, 8). Данная обработка неоднозначности позволяет более точно формировать стратегию игры.

5) Игра заканчивается поражением, если после очередного хода все клетки заполнены


Первая версия игры:

Идея заключалась в том, чтобы получить "сырую" версию игры. Программа должна была уметь двигать блоки с числами в одну сторону ( мы выбрали правую), создавать блоки с двойками при смещении вправо.

Начали мы с определения класса Сell - клетки, в которой хранится число
________________________________________________________________________
------------------------------------------------------------------------

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

    int get_bin(){return b_pow;}
    int get_color(){return Set_color();}

private:
    int Set_color();
    int b_pow;
};

#endif // CELL_H

-------------------------------------------------------------------------

#include "cell.h"

int Cell::Set_color()

{
   
   switch(b_pow)  
   {
   
    case 0: 
        return 17;
        break;    
    case 1:
        return 47;
        break;
    case 2:
        return 19;
        break;
    case 3:
        return 122;
        break;
    case 4:
        return 131;
        break;
    case 5:
        return 89;
        break;
    case 6:
        return 2;
    default:
        return 63;
    }
}


Cell::Cell(int bin)

{
  b_pow=bin;
}


------------------------------------------------------------------------
-----------------------------------------------------------------------

Определим класс Board - доску, на которой проходит игра, и которая храниьт клетки Cell и отвечае

#ifndef BOARD_H

#define BOARD_H

#include "../Graph_lib/Graph.h"

#include "cell.h"

using namespace Graph_lib;

class Board : public Shape

{

public:

    Board();
    void set_table(char move);

    //static void cb_r (Address , Address );
    void right_set(); //придумать как убрать это в приват
    void left_set();

protected:

    void draw_lines() const;
private:

    void up_set(int i);
    void down_set(int i);


    void random_cell();

    std::vector<Vector_ref<Cell>> table;
    bool can_u{true}, can_d{true};
    bool can_r{true}, can_l{true};
};

#endif // BOARD_H



------------------------------------------------------------

#include "board.h"
#include <ctime>
#include "../Graph_lib/Graph.h"

Board::Board()
    :Shape ()
{
    for (int i{0} ; i<4 ; ++i)
    {
        Vector_ref<Cell> set;
        table.push_back(set);
        for (int j{0} ; j<4 ; ++j)
        {
            table[i].push_back(new Cell(0));
        }
    }
    random_cell();
    random_cell();
}


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
        throw ("Game Over"); //if no moves on next turn
    }

    int a = free_cell.size();

    if (a-1 == 0)
    {
        table[free_cell[0][0]][free_cell[0][1]]=Cell{1};
        return;
    }

    srand((unsigned int)time(NULL));
    a = abs(rand() % a);
    table[free_cell[a][0]][free_cell[a][1]]=Cell{1};
}


void Board::up_set(int i) //not done
{

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
                    table[i][j-1]=Cell(0);
                }
            }
        }
    }
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

        for (int j{0} ; j<3 ; ++j)//слияние влево
        {
            if (table[i][j].get_bin()==table[i][j+1].get_bin() && table[i][j].get_bin()!=0)
            {
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

}


void Board::set_table(char move)
{
    switch (move)
    {
    case 'r':
    {
        right_set();

        random_cell();
        break;
    }
    case 'l':
        left_set();

        random_cell();
        break;



    case 'u':
        for (int i{0} ; i<4 ; i++)
            up_set(i);
        random_cell();
        break;

    }
}


void Board::draw_lines() const
{
    Shape::draw_lines();

    Vector_ref<Graph_lib::Rectangle> rect;
    Vector_ref<Text> box;
    std::vector<Vector_ref<Cell>> prin = table;

    for (int i{0} ; i<4 ; i++)
    {
        for (int j{0} ; j<4 ; j++)
        {
            rect.push_back(new Graph_lib::Rectangle{Point{j*200,i*200},200,200});
            rect[rect.size()-1].set_fill_color(prin[i][j].get_color());
            rect[rect.size()-1].draw();

            if (prin[i][j].get_bin()>0)
            {
               std::string s = std::to_string((int)pow(2,prin[i][j].get_bin()));
               box.push_back(new Graph_lib::Text{Point{j*200+80, i*200+120} , s });
               box[box.size()-1].set_font_size(80);
               box[box.size()-1].draw();
            }
        }
    }
}

___________________________________________________________________________________




