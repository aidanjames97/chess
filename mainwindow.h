#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include "tile.h"

using namespace std;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QGridLayout *board;
    Tile *boardArr[8][8];
private:
    Tile *exClicked; // last clicked tile by player
private slots:
    void handleTile(Tile *tile);
};

#endif // MAINWINDOW_H