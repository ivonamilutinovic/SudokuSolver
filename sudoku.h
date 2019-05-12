#ifndef SUDOKU_H
#define SUDOKU_H
#include <QGraphicsWidget>
#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <iostream>
#include <vector>
#define NUMBERS (9) // sudoku je dimenzije 9x9
#define SQR (3)     // kvadrati su dimenzije 3x3



class Sudoku : public QGraphicsWidget
{
public:

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;


    Sudoku();                                    // konstruktor koji sve elemente postavlja na nulu
    Sudoku(const Sudoku& other);
    Sudoku& operator= (const Sudoku& other);

    ~Sudoku() = default;
    void write_in(unsigned value, int i, int j); // funkcija upisuje vrednost value na poziciju [i][j]
    void solve();                                // funkcija resava sudoku ili ispisuje da sudoku nema resenje



private:

    bool writeInRow();
    bool writeInColl();
    bool writeInBlock();
    bool isCandidate(int i, int j, int value);
    bool writeOnPos();
    bool backtracking(int i, int j);
    void delete_in(unsigned value, int i, int j);

    unsigned _elements[NUMBERS][NUMBERS];       // cuva elemente sudokua
    bool     _inRow[NUMBERS][NUMBERS];          // cuva informaciju da li i. red sadrzi broj j+1
    bool     _inColl[NUMBERS][NUMBERS];         // cuva informaciju da li i. kolona sadrzi broj j+1
    bool     _inBlock[NUMBERS][NUMBERS];        // cuva informaciju da li i. blok sadrzi broj sadrzi broj j+1.
    bool     _newNumber[NUMBERS][NUMBERS];
    bool     _solutionId = true;
    bool     _validId    = true;
    // Svaki blok je kvadrat dimenzije 3x3, oznake blokova:
    // 0 1 2
    // 3 4 5
    // 6 7 8

    friend std::ostream& operator<< (std::ostream &ostr,const Sudoku &m);


};

#endif // SUDOKU_H
