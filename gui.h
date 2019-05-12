#ifndef GUI_H
#define GUI_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QFileDialog>
#include "sudoku.h"
#include "image.h"
#include "solution.h"

namespace Ui {
class GUI;
}

class GUI : public QWidget
{
    Q_OBJECT

public:
    explicit GUI(QWidget *parent = 0);
    ~GUI();

private slots:
    void on_browseButton_clicked();
    void on_solveButton_clicked();
    void on_lineEdit_returnPressed();

private:
    Ui::GUI *ui;
    QGraphicsScene scene;
    Sudoku sudoku;
};

#endif // GUI_H
