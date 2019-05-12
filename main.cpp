#include "gui.h"
#include <QtGui>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "sudoku.h"
#include "image.h"
#include <QPushButton>
#include <QFont>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GUI w;
    w.show();

    return a.exec();
}
