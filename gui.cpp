#include "gui.h"
#include "ui_gui.h"
#include <vector>

GUI::GUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GUI)
{
    ui->setupUi(this);

}

GUI::~GUI()
{
    delete ui;
}


void GUI::on_browseButton_clicked()
{
    QString text = QFileDialog::getOpenFileName(ui->browseButton, "Open a file", "/home",
         "Images (*.bmp)");
    ui->lineEdit->setText(text);
    if(text == nullptr)
        return;
    
    //otvaramo fajl i prepoznajemo sudoku
    std::vector<std::vector<unsigned char> > matrica = readImage(text.toStdString());
    //brisemo prethodne vrednosti iz sudokua
    sudoku = Sudoku();

    lineRecognition(sudoku,matrica);

    //nakon sto smo prepoznali sudoku, ispisujemo ga
    scene.setSceneRect(40,100, 400, 400);
    sudoku.setPos(53,118);
    scene.addItem(&sudoku);

    QPixmap originalImage(":/images/scene1.jpeg");
    QPixmap scaledImage = originalImage.scaled(ui->view->width(), ui->view->height());
    ui->view->setBackgroundBrush(scaledImage);

    ui->view->setScene(&scene);
    ui->view->show();

}

void GUI::on_solveButton_clicked()
{
    sudoku.solve();
    scene.removeItem(&sudoku);
    Solution* s = new Solution(sudoku);
    s->show();
    ui->view->close();

}

void GUI::on_lineEdit_returnPressed()
{
    QString s = ui->lineEdit->text();

    //brisemo prethodne vrednosti iz sudokua
    sudoku=Sudoku();
    //otvaramo fajl i prepoznajemo sudoku
    std::vector<std::vector<unsigned char> > matrica = readImage(s.toStdString());
    lineRecognition(sudoku,matrica);

    //nakon sto smo prepoznali sudoku, ispisujemo ga
    scene.setSceneRect(40,100, 400, 400);
    sudoku.setPos(53,118);
    scene.addItem(&sudoku);

    QPixmap originalImage(":/images/scene1.jpeg");
    QPixmap scaledImage = originalImage.scaled(ui->view->width(), ui->view->height());
    ui->view->setBackgroundBrush(scaledImage);

    ui->view->setScene(&scene);
    ui->view->show();
}
