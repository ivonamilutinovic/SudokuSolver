#include "solution.h"
#include "ui_solution.h"


Solution::Solution(Sudoku& sudoku, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Solution),sudoku(sudoku)
{
    ui->setupUi(this);

    scene.setSceneRect(150, 150, 400, 400);
    scene.setItemIndexMethod(QGraphicsScene::NoIndex);

    sudoku.setPos(190,192);
    scene.addItem(&sudoku);

    ui->view2->setScene(&scene);
    ui->view2->show();
}

Solution::~Solution()
{
    delete ui;
}

void Solution::on_again_clicked()
{
    //zatvara se tekuca scena

    this->close();
}
