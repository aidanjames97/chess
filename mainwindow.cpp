#include <iostream>
#include <QApplication>
#include "mainwindow.h"

#define WH 8
#define TILE_SIZE 100

// constructor
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    board = new QGridLayout(this);
    board->setGeometry(QRect(QPoint(0,0), QSize(WH*TILE_SIZE, WH*TILE_SIZE)));
    exClicked = nullptr;

    for(int i=0; i<8;i++) {
        for(int j=0; j<8;j++) {
            Tile *tile = new Tile(this);
            tile->setGeometry(QRect(QPoint(i*TILE_SIZE, j*TILE_SIZE), QSize(TILE_SIZE, TILE_SIZE)));
            tile->setIconSize(QSize(TILE_SIZE - 10, TILE_SIZE - 10));
            tile->setLoc(make_pair(i,j));
            boardArr[i][j] = tile; // adding tile to board array
            // set background color
            if((j%2 == 1) ^ (i%2 == 1)) {
                // odd
                tile->setGreen();
            }
            // setting correct icon and enum state
            if(j==0) {
                tile->setTeam(Team::black);
                if(i==1 || i==6) {
                    // black knight
                    tile->setKnight();
                } else if(i==2 || i==5) {
                    // black bishop
                    tile->setBishop();
                } else if(i==3) {
                    // black queen
                    tile->setQueen();
                } else if(i==4) {
                    //  black king
                    tile->setKing();
                } else {
                    // black rook
                    tile->setRook();
                }
            } else if(j==1) {
                tile->setTeam(Team::black);
                // black pawn
                tile->setPawn();
            } else if(j==6) {
                tile->setTeam(Team::white);
                // white pawn
                tile->setPawn();
            } else if(j==7) {
                tile->setTeam(Team::white);
                if(i==1 || i==6) {
                    // white knight
                    tile->setKnight();
                } else if(i==2 || i==5) {
                    // white bishop
                    tile->setBishop();
                } else if(i==3) {
                    // white queen
                    tile->setQueen();
                } else if(i==4) {
                    //  white king
                    tile->setKing();
                } else {
                    // white rook
                    tile->setRook();
                }
            }

            board->addWidget(tile, i, j);
            connect(tile, &Tile::leftClick, this, [this, tile]() { handleTile(tile);});
        }
    }
}

// handles mouse click on tiles
void MainWindow::handleTile(Tile *tile) {
    // remove any coloring on tiles and reset vector
    for(auto t : resetAfter) {
        t->removeColor();
    }
    resetAfter.clear();

    // check in possile moves player could have made with tile clicks
    // (this means player is going to move the that space)
    if(exClicked == nullptr) {
        return;
    }

    // BUG IN HERE WITH CLICKED BELIVE TO BE EX CLICKED OR THIS LOOP BELOW

    // searing in moves in
    for(auto t : movePossible) {
        if(tile == t) {
            // move piece to this location
            return;
        }
    }

    // defaut tasks handled above ----

    // check for no piece on clicked tile
    if(tile->getType() == Type::none) {
        return;
    }

    // piece found, find possible moves and color them

    int x = tile->getLoc().first; // holds curr tile x
    int y = tile->getLoc().second; // holds curr tile y
    resetAfter.push_back(tile); // add this tile to the reset vector
    exClicked = tile; // set last clicked to current tile

    if(tile->getTeam() == Team::white) {
        // white peice (bot going up)
        switch(tile->getType()) {
            case(Type::none):
                break;
            case(Type::pawn):
                if(y-1 == 0) {
                    // hit bottom of board (promo)
                    return;
                }
                if(boardArr[x][y-1]->getType() == Type::none) {
                    // can move forward
                    boardArr[x][y-1]->setBlue();
                    resetAfter.push_back(boardArr[x][y-1]);
                    movePossible.push_back(boardArr[x][y-1]);
                    if(y==6 && boardArr[x][y-2]->getType() == Type::none) {
                        // on starting row (can move up 2) and no one in front
                        boardArr[x][y-2]->setBlue();
                        resetAfter.push_back(boardArr[x][y-2]);
                        movePossible.push_back(boardArr[x][y-2]);
                    }
                }
                break;
        }
        return;
    }

    // black peice (top going down)
    switch(tile->getType()) {
        case(Type::none):
            break;
        case(Type::pawn):
            if(y == 7) {
                // hit top of board (promo)
                return;
            }
            if(boardArr[x][y+1]->getType() == Type::none) {
                // can move forward
                boardArr[x][y+1]->setBlue();
                resetAfter.push_back(boardArr[x][y+1]);
                movePossible.push_back(boardArr[x][y+1]);
                if(y==1 && boardArr[x][y+2]->getType() == Type::none) {
                    // on starting row (can move up 2) and no one in front
                    boardArr[x][y+2]->setBlue();
                    resetAfter.push_back(boardArr[x][y+2]);
                    movePossible.push_back(boardArr[x][y+2]);
                }
            }
    }
}

// deconstructor
MainWindow::~MainWindow() {}