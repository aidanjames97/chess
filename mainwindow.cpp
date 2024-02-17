#include <iostream>
#include <QApplication>
#include "mainwindow.h"

#define WH 8
#define TILE_SIZE 100

// constructor
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    board = new QGridLayout(this);
    board->setGeometry(QRect(QPoint(0,0), QSize(WH*TILE_SIZE, WH*TILE_SIZE)));

    exClicked = nullptr; // default ex click to null

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
    if(exClicked != nullptr) {
        exClicked->removeYellow();
    }
    boardArr[(tile->getLoc().first) - 1][tile->getLoc().second]->setBlue();
    exClicked = tile;
    return;
}

// deconstructor
MainWindow::~MainWindow() {}