#ifndef GRIDLAYOUT_H
#define GRIDLAYOUT_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <iostream>
#include <QApplication>
#include "tile.h"

using namespace std;

class GridLayout : public QGridLayout {
    Q_OBJECT
public:
    GridLayout(QWidget *parent = nullptr);
    ~GridLayout();
    Tile *boardArr[8][8];
private:
    vector<Tile*> movePossible; // holds moves the player is allowed to make
    vector<Tile*> resetAfter; // holds tiles whos colors need to be reset
    Tile* exClicked; // to hold tile player clicked before for icon movement
    bool moved = false;
    // for possible moves
    void checkUp(int x, int y, Tile* checking, Tile *tile); // checks for possible move up
    void checkDown(int x, int y, Tile* checking, Tile *tile); // checks for possible move down
    void checkLeft(int x, int y, Tile* checking, Tile *tile); // checks for possible move left
    void checkRight(int x, int y, Tile* checking, Tile *tile); // checks for possible move right
    void checkLD(int x, int y, Tile* checking, Tile *tile); // checks for possible move left down
    void checkLU(int x, int y, Tile* checking, Tile *tile); // checks for possible move left up
    void checkRD(int x, int y, Tile* checking, Tile *tile); // checks for possible move right down
    void checkRU(int x, int y, Tile* checking, Tile *tile); // checks for possible move right up
    void checkBlackPawn(int x, int y, Tile* checking, Tile *tile); // checks for possible black pawn moves
    void checkWhitePawn(int x, int y, Tile* checking, Tile *tile); // checks for possible white pawn moves
    void checkKing(int x, int y, Tile* checking, Tile *tile); // checks for possible king moves
    void checkKnight(int x, int y, Tile* checking, Tile *tile); // checks for possible knight moves
    bool whoTurn = true; // true is white, false is black
    vector<Type> whiteTaken;
    vector<Type> blackTaken;
    void gameOver(); // game is over, 1 for white, 2 for black
private slots:
    void handleTile(Tile *tile);
};

#endif // GRIDLAYOUT_H