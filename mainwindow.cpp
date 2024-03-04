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
            } else {
                tile->setTeam(Team::none);
            }

            board->addWidget(tile, i, j);
            connect(tile, &Tile::leftClick, this, [this, tile]() { handleTile(tile);});
        }
    }
}

// handles mouse click on tiles
void MainWindow::handleTile(Tile *tile) {
    // handles when user clicks same piece when it is already selected
    if(tile == exClicked && !moved) {
        return;
    }
    tile->setYellow(); // set background yellow (indicate click)
    // remove any coloring on tiles
    for(auto t : resetAfter) {
        t->removeColor();
    }
    resetAfter.clear(); // clear array

    // searing in moves in
    for(auto t : movePossible) {
        if(tile == t) {
            // move piece to this location
            Type tmp = exClicked->getType();
            tile->setTeam(exClicked->getTeam());
            exClicked->removePiece();
            switch (tmp) {
                case(Type::pawn):
                    tile->setPawn();
                    break;
                case(Type::bishop):
                    tile->setBishop();
                    break;
                case(Type::knight):
                    tile->setKnight();
                    break;
                case(Type::rook):
                    tile->setRook();
                    break;
                case(Type::queen):
                    tile->setQueen();
                    break;
                default:
                    tile->setKing();
                    break;
            }
            tile->removeColor(); // remove background coloring
            exClicked = tile; // last clicked tile is now this tile
            movePossible.clear(); // clearing moves which were possible for last piece clicked
            moved = true; // player has moved
            return;
        }
    }
    moved = false; // player has not moved
    movePossible.clear(); // clearing moves which were possible for last piece clicked

    // defaut tasks handled above ----

    int x = tile->getLoc().first; // holds curr tile x
    int y = tile->getLoc().second; // holds curr tile y
    // tmp x and y so we can manipulate
    int tx = x; 
    int ty = y;
    resetAfter.push_back(tile); // add this tile to the reset vector
    exClicked = tile; // set last clicked to current tile
    Tile* checking = new Tile(); // tile pointer to tile we are checking

    // check which piece (if one) player has clicked (white)
    if(tile->getTeam() == Team::white) {
        // white piece
        switch(tile->getType()) {
            case(Type::none):
                return; // player clicked a blank space
            case(Type::pawn):
                // player clicked pawn
                if(y == 0) {
                    // hit bottom of board (promo)
                    cout << " [ PROMOTION ]" << endl;
                    return;
                }
                checking = boardArr[x][y-1];
                if(checking->getType() == Type::none) {
                    // can move forward
                    checking->setBlue();
                    resetAfter.push_back(checking);
                    movePossible.push_back(checking);
                    checking = boardArr[x][y-2];
                    if(y==6 && checking->getType() == Type::none) {
                        // on starting row (can move up 2) and no one in front
                        checking->setBlue();
                        resetAfter.push_back(checking);
                        movePossible.push_back(checking);
                    }
                }
                // check for piece to the side and checking for off board
                checking = boardArr[x+1][y-1];
                if(x+1 <= 7) {
                    if(checking->getType() != Type::none && checking->getTeam() != tile->getTeam()) {
                        checking->setBlue();
                        resetAfter.push_back(checking);
                        movePossible.push_back(checking);
                    }
                }
                checking = boardArr[x-1][y-1];
                if(x-1 >= 0) {
                    if(checking->getType() != Type::none && checking->getTeam() != tile->getTeam()) {
                        checking->setBlue();
                        resetAfter.push_back(checking);
                        movePossible.push_back(checking);
                    }
                }
                break;
            case(Type::bishop):
                // player clicked bishop
                tx = x;
                ty = y;
                // check left up diagonal
                while(tx > 0 && ty > 0) {
                    tx--;
                    ty--;
                    checking = boardArr[tx][ty];
                    // checking for teamate in way
                    if(checking->getTeam() == tile->getTeam()) {
                        break;
                    }

                    // at this point path is clear continue to check next
                    checking->setBlue();
                    resetAfter.push_back(checking);
                    movePossible.push_back(checking);
                    // checking for enemy in way
                    if(checking->getType() != Type::none) {
                        break;
                    }
                }
                tx = x;
                ty = y;
                // check right up diagonal
                while(tx > 0 && ty < 7) {
                    tx--;
                    ty++;
                    checking = boardArr[tx][ty];
                    // checking for teamate in way
                    if(checking->getTeam() == tile->getTeam()) {
                        break;
                    }
                    // at this point path is clear continue to check next
                    checking->setBlue();
                    resetAfter.push_back(checking);
                    movePossible.push_back(checking);
                    // checking for enemy in way
                    if(checking->getType() != Type::none) {
                        break;
                    }
                }
                tx = x;
                ty = y;
                // check right down diagonal
                while(tx < 7 && ty < 7) {
                    tx++;
                    ty++;
                    checking = boardArr[tx][ty];
                    // checking for teamate in way
                    if(checking->getTeam() == tile->getTeam()) {
                        break;
                    }
                    // at this point path is clear continue to check next
                    checking->setBlue();
                    resetAfter.push_back(checking);
                    movePossible.push_back(checking);
                    // checking for enemy in way
                    if(checking->getType() != Type::none) {
                        break;
                    }
                }
                tx = x;
                ty = y;
                // check left down diagonal
                while(tx < 7 && ty > 0) {
                    tx++;
                    ty--;
                    checking = boardArr[tx][ty];
                    // checking for teamate in way
                    if(checking->getTeam() == tile->getTeam()) {
                        break;
                    }
                    // at this point path is clear continue to check next
                    checking->setBlue();
                    resetAfter.push_back(checking);
                    movePossible.push_back(checking);
                    // checking for enemy in way
                    if(checking->getType() != Type::none) {
                        break;
                    }
                }
                break;
            case(Type::knight):
                // player clicked knight
                cout << "white knight" << endl;
                break;
            case(Type::rook):
                tx = x;
                ty = y;
                // check up
                while(ty > 0){
                    ty--;
                    checking = boardArr[tx][ty];
                    if(checking->getTeam() == tile->getTeam()) {
                        break;
                    }
                    checking->setBlue();
                    resetAfter.push_back(checking);
                    movePossible.push_back(checking);
                    // checking for enemy in the way
                    if(checking->getType() != Type::none) {
                        break; // enemy here break looping
                    }
                }
                tx = x;
                ty = y;
                // check down
                while(ty < 7){
                    ty++;
                    checking = boardArr[tx][ty];
                    if(checking->getTeam() == tile->getTeam()) {
                        break;
                    }
                    checking->setBlue();
                    resetAfter.push_back(checking);
                    movePossible.push_back(checking);
                    // checking for enemy in the way
                    if(checking->getType() != Type::none) {
                        break; // enemy here break looping
                    }
                }
                tx = x;
                ty = y;
                // check left
                while(tx > 0){
                    tx--;
                    checking = boardArr[tx][ty];
                    if(checking->getTeam() == tile->getTeam()) {
                        break;
                    }
                    checking->setBlue();
                    resetAfter.push_back(checking);
                    movePossible.push_back(checking);
                    // checking for enemy in the way
                    if(checking->getType() != Type::none) {
                        break; // enemy here break looping
                    }
                }
                tx = x;
                ty = y;
                // check right
                while(tx < 7){
                    tx++;
                    checking = boardArr[tx][ty];
                    if(checking->getTeam() == tile->getTeam()) {
                        break;
                    }
                    checking->setBlue();
                    resetAfter.push_back(checking);
                    movePossible.push_back(checking);
                    // checking for enemy in the way
                    if(checking->getType() != Type::none) {
                        break; // enemy here break looping
                    }
                }
                break;
            case(Type::queen):
                cout << "white queen" << endl;
                break;
            case(Type::king):
                cout << "white king" << endl;;
                break;
        }
        return;
    }

    // check which piece (if one) player has clicked (black)
    if(tile->getTeam() == Team::black) {
        // white peice (bot going up)
        switch(tile->getType()) {
            case(Type::none):
                return;
            case(Type::pawn):
                cout << "black pawn" << endl;
                if(y == 7) {
                    // hit bottom of board (promo)
                    return;
                }
                checking = boardArr[x][y+1];
                if(checking->getType() == Type::none) {
                    // can move forward
                    checking->setBlue();
                    resetAfter.push_back(checking);
                    movePossible.push_back(checking);
                    checking = boardArr[x][y+2];
                    if(y==1 && checking->getType() == Type::none) {
                        // on starting row (can move up 2) and no one in front
                        checking->setBlue();
                        resetAfter.push_back(checking);
                        movePossible.push_back(checking);
                    }
                }
                // check for piece to the side and checking for off board
                checking = boardArr[x+1][y+1];
                if(x+1 <= 7) {
                    if(checking->getType() != Type::none && checking->getTeam() != tile->getTeam()) {
                        checking->setBlue();
                        resetAfter.push_back(checking);
                        movePossible.push_back(checking);
                    }
                }
                checking = boardArr[x-1][y+1];
                if(x-1 >= 0) {
                    if(checking->getType() != Type::none && checking->getTeam() != tile->getTeam()) {
                        checking->setBlue();
                        resetAfter.push_back(checking);
                        movePossible.push_back(checking);
                    }
                }
                break;
            case(Type::bishop):
                tx = x;
                ty = y;
                // check left up diagonal
                while(tx > 0 && ty > 0) {
                    tx--;
                    ty--;
                    checking = boardArr[tx][ty];
                    // checking for teamate in way
                    if(checking->getTeam() == tile->getTeam()) {
                        break;
                    }

                    // at this point path is clear continue to check next
                    checking->setBlue();
                    resetAfter.push_back(checking);
                    movePossible.push_back(checking);
                    // checking for enemy in way
                    if(checking->getType() != Type::none) {
                        break;
                    }
                }
                tx = x;
                ty = y;
                // check right up diagonal
                while(tx > 0 && ty < 7) {
                    tx--;
                    ty++;
                    checking = boardArr[tx][ty];
                    // checking for teamate in way
                    if(checking->getTeam() == tile->getTeam()) {
                        break;
                    }
                    // at this point path is clear continue to check next
                    checking->setBlue();
                    resetAfter.push_back(checking);
                    movePossible.push_back(checking);
                    // checking for enemy in way
                    if(checking->getType() != Type::none) {
                        break;
                    }
                }
                tx = x;
                ty = y;
                // check right down diagonal
                while(tx < 7 && ty < 7) {
                    tx++;
                    ty++;
                    checking = boardArr[tx][ty];
                    // checking for teamate in way
                    if(checking->getTeam() == tile->getTeam()) {
                        break;
                    }
                    // at this point path is clear continue to check next
                    checking->setBlue();
                    resetAfter.push_back(checking);
                    movePossible.push_back(checking);
                    // checking for enemy in way
                    if(checking->getType() != Type::none) {
                        break;
                    }
                }
                tx = x;
                ty = y;
                // check left down diagonal
                while(tx < 7 && ty > 0) {
                    tx++;
                    ty--;
                    checking = boardArr[tx][ty];
                    // checking for teamate in way
                    if(checking->getTeam() == tile->getTeam()) {
                        break;
                    }
                    // at this point path is clear continue to check next
                    checking->setBlue();
                    resetAfter.push_back(checking);
                    movePossible.push_back(checking);
                    // checking for enemy in way
                    if(checking->getType() != Type::none) {
                        break;
                    }
                }
                break;
            case(Type::knight):
                cout << "black knight" << endl;
                break;
            case(Type::rook):
                tx = x;
                ty = y;
                // check up
                while(ty > 0){
                    ty--;
                    checking = boardArr[tx][ty];
                    if(checking->getTeam() == tile->getTeam()) {
                        break;
                    }
                    checking->setBlue();
                    resetAfter.push_back(checking);
                    movePossible.push_back(checking);
                    // checking for enemy in the way
                    if(checking->getType() != Type::none) {
                        break; // enemy here break looping
                    }
                }
                tx = x;
                ty = y;
                // check down
                while(ty < 7){
                    ty++;
                    checking = boardArr[tx][ty];
                    if(checking->getTeam() == tile->getTeam()) {
                        break;
                    }
                    checking->setBlue();
                    resetAfter.push_back(checking);
                    movePossible.push_back(checking);
                    // checking for enemy in the way
                    if(checking->getType() != Type::none) {
                        break; // enemy here break looping
                    }
                }
                tx = x;
                ty = y;
                // check left
                while(tx > 0){
                    tx--;
                    checking = boardArr[tx][ty];
                    if(checking->getTeam() == tile->getTeam()) {
                        break;
                    }
                    checking->setBlue();
                    resetAfter.push_back(checking);
                    movePossible.push_back(checking);
                    // checking for enemy in the way
                    if(checking->getType() != Type::none) {
                        break; // enemy here break looping
                    }
                }
                tx = x;
                ty = y;
                // check right
                while(tx < 7){
                    tx++;
                    checking = boardArr[tx][ty];
                    if(checking->getTeam() == tile->getTeam()) {
                        break;
                    }
                    checking->setBlue();
                    resetAfter.push_back(checking);
                    movePossible.push_back(checking);
                    // checking for enemy in the way
                    if(checking->getType() != Type::none) {
                        break; // enemy here break looping
                    }
                }
                break;
            case(Type::queen):
                cout << "black queen" << endl;
                break;
            case(Type::king):
                cout << "black king" << endl;
                break;
            
        }
        return;
    }
}

// deconstructor
MainWindow::~MainWindow() {}