#include "gridlayout.h"
#include <QApplication>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // init QT application
    a.setWindowIcon(QIcon(QString("./../../../pieces/wK.svg")));

    QWidget w; // window object
    GridLayout *grid = new GridLayout(&w);
    w.setLayout(grid);
    w.setWindowTitle("Chess"); // title of window
    w.setFixedSize(800, 800);
    w.setContentsMargins(0,0,0,0);

    w.setStyleSheet("background-color: #C0C0C0");    
    w.show(); // show window
    return a.exec(); // run app
}