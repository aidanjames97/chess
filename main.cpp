#include "mainwindow.h"
#include <QApplication>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    cout << "\n=====\n" << endl;

    QApplication a(argc, argv); // init QT application

    cout << " -=-=-=-=-=" << endl;

    MainWindow w; // window object

    cout << "\n=====\n" << endl;

    w.setWindowTitle("Chess"); // title of window
    w.setFixedSize(800, 800);
    w.setStyleSheet("background-color: #C0C0C0");
    w.show(); // show window
    return a.exec(); // run app
}