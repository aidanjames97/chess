#include "gridlayout.h"
#include <QApplication>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // init QT application
    a.setWindowIcon(QIcon(QString("./../../../pieces/wK.svg")));

    QWidget w; // window object
    GridLayout *grid = new GridLayout(&w); // new grid layout obj
    w.setWindowTitle("Chess"); // title of window
    w.setFixedSize(800, 800); // window cannot change from 800,800
    grid->setContentsMargins(0,0,0,0); // setting margins for widgets
    w.setLayout(grid); // set the window layout to the grid
    w.layout()->setSpacing(0); // set spacing of grid on window to 0

    w.setStyleSheet("background-color: #C0C0C0"); // background is grey  
    w.show(); // show window
    return a.exec(); // run app
}