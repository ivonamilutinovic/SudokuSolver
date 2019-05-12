#ifndef SOLUTION_H
#define SOLUTION_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "gui.h"


namespace Ui {
class Solution;
}

class Solution : public QWidget
{
    Q_OBJECT

public:
    ~Solution();
    explicit Solution(Sudoku& sudoku, QWidget *parent = 0);

private slots:
    void on_again_clicked();

private:
    Ui::Solution *ui;
    Sudoku sudoku;
    QGraphicsScene scene;
};

#endif // SOLUTION_H
