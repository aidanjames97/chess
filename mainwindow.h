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
    vector<Tile*> movePossible; // holds moves the player is allowed to make
    vector<Tile*> resetAfter; // holds tiles whos colors need to be reset
    Tile* exClicked; // to hold tile player clicked before for icon movement
    bool moved = false;
private slots:
    void handleTile(Tile *tile);
};

#endif // MAINWINDOW_H