#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // init QT application
    MainWindow w; // window object
    w.setWindowTitle("Chess"); // title of window
    w.setFixedSize(800, 800);
    w.setStyleSheet("background-color: #C0C0C0");
    w.show(); // show window
    return a.exec(); // run app
}