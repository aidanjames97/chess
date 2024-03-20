#include <iostream>
#include <QApplication>
#include "mainwindow.h"

#define WH 8
#define TILE_SIZE 100

// constructor
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    cout << "start of main window" << endl;
    board = new QGridLayout(this);
    cout << "here" << endl;
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

    cout << "main window constructor finsihsed" << endl;
}

// handles mouse click on tiles
void MainWindow::handleTile(Tile *tile) {
    // searing in moves in
    for(auto t : movePossible) {
        if(tile == t) {
            // move piece to this location
            Type tmp = exClicked->getType();

            // check if we are eliminating opponent piece
            if(tile->getTeam() != exClicked->getTeam()) {
                if(tile->getTeam() == Team::white) {
                    whiteTaken.push_back(tile->getType());
                } else {
                    blackTaken.push_back(tile->getType());
                }
            }

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
            whoTurn = !whoTurn;
        }
    }
    movePossible.clear(); // clearing moves which were possible for last piece clicked

    // remove any coloring on previous tiles
    for(auto t : resetAfter) {
        t->removeColor();
    }
    resetAfter.clear(); // clear array

    // check if it is that players turn
    // handles when user clicks same piece when it is already selected
    if((tile->getTeam() == Team::none) || ((whoTurn && tile->getTeam() == Team::black) || (!whoTurn && tile->getTeam() == Team::white))) {
        return;
    }

    moved = false; // player didnt move on click

    tile->setYellow(); // set background yellow (indicate click)
    resetAfter.push_back(tile); // add this tile to the reset vector

    int x = tile->getLoc().first; // holds curr tile x
    int y = tile->getLoc().second; // holds curr tile y

    exClicked = tile; // set last clicked to current tile
    Tile* checking = new Tile(); // tile pointer to tile we are checking

    // check which piece (if one) player has clicked (white)
    if(tile->getTeam() == Team::white) {
        // white piece
        switch(tile->getType()) {
            case(Type::none):
                return;
            case(Type::pawn):
                checkWhitePawn(x, y, checking, tile);
                break;
            case(Type::bishop):
                checkLU(x, y, checking, tile);
                checkRU(x, y, checking, tile);
                checkRD(x, y, checking, tile);
                checkLD(x, y, checking, tile);
                break;
            case(Type::knight):
                // player clicked knight
                checkKnight(x, y, checking, tile);
                break;
            case(Type::rook):
                checkUp(x, y, checking, tile);
                checkDown(x, y, checking, tile);
                checkLeft(x, y, checking, tile);
                checkRight(x, y, checking, tile);
                break;
            case(Type::queen):
                checkLU(x, y, checking, tile);
                checkRU(x, y, checking, tile);
                checkRD(x, y, checking, tile);
                checkLD(x, y, checking, tile);
                checkUp(x, y, checking, tile);
                checkDown(x, y, checking, tile);
                checkLeft(x, y, checking, tile);
                checkRight(x, y, checking, tile);
                break;
            case(Type::king):
                break;
        }
    }
    
    // check which piece (if one) player has clicked (black)
    if(tile->getTeam() == Team::black) {
        // white peice (bot going up)
        switch(tile->getType()) {
            case(Type::none):
                return;
            case(Type::pawn):
                checkBlackPawn(x, y, checking, tile);
                break;
            case(Type::bishop):
                checkLU(x, y, checking, tile);
                checkRU(x, y, checking, tile);
                checkRD(x, y, checking, tile);
                checkLD(x, y, checking, tile);
                break;
            case(Type::knight):
                checkKnight(x, y, checking, tile);
                break;
            case(Type::rook):
                checkUp(x, y, checking, tile);
                checkDown(x, y, checking, tile);
                checkLeft(x, y, checking, tile);
                checkRight(x, y, checking, tile);
                break;
            case(Type::queen):
                checkLU(x, y, checking, tile);
                checkRU(x, y, checking, tile);
                checkRD(x, y, checking, tile);
                checkLD(x, y, checking, tile);
                checkUp(x, y, checking, tile);
                checkDown(x, y, checking, tile);
                checkLeft(x, y, checking, tile);
                checkRight(x, y, checking, tile);
                break;
            case(Type::king):
                break;
            
        }
    }

    // cout << "whiteTaken: ";
    // for(auto y : whiteTaken) {
    //     switch (y) {
    //     case (Type::pawn):
    //         cout << "WP ";
    //         break;
    //     case (Type::bishop):
    //         cout << "WB ";
    //         break;
    //     case (Type::knight):
    //         cout << "WN ";
    //         break;
    //     case (Type::rook):
    //         cout << "WR ";
    //         break;
    //     case (Type::queen):
    //         cout << "WQ ";
    //         break;
    //     case (Type::king):
    //         cout << "WK ";
    //         break;
    //     }
    // }

    // cout << "\nblackTaken: ";
    // for(auto y : blackTaken) {
    //     switch (y) {
    //     case (Type::pawn):
    //         cout << "BP ";
    //         break;
    //     case (Type::bishop):
    //         cout << "BB ";
    //         break;
    //     case (Type::knight):
    //         cout << "BN ";
    //         break;
    //     case (Type::rook):
    //         cout << "BR ";
    //         break;
    //     case (Type::queen):
    //         cout << "BQ ";
    //         break;
    //     case (Type::king):
    //         cout << "BK ";
    //         break;
    //     }
    // }
    // cout << "\n------" << endl;
};

// ---- FOR CHECKING PIECE MOVEMENT ----
// checking up
void MainWindow::checkUp(int x, int y, Tile* checking, Tile *tile) {
    int tx = x;
    int ty = y;
    // check up
    while(ty > 0){
        ty--;
        checking = boardArr[tx][ty];
        if(checking->getTeam() == tile->getTeam()) {
            return;
        }
        checking->setBlue();
        resetAfter.push_back(checking);
        movePossible.push_back(checking);
        // checking for enemy in the way
        if(checking->getType() != Type::none) {
            return; // enemy here break looping
        }
    }
}

// checking down
void MainWindow::checkDown(int x, int y, Tile* checking, Tile *tile) {
    int tx = x;
    int ty = y;
    // check up
    while(ty < 7){
        ty++;
        checking = boardArr[tx][ty];
        if(checking->getTeam() == tile->getTeam()) {
            return;
        }
        checking->setBlue();
        resetAfter.push_back(checking);
        movePossible.push_back(checking);
        // checking for enemy in the way
        if(checking->getType() != Type::none) {
            return; // enemy here break looping
        }
    }
}

// checking left
void MainWindow::checkLeft(int x, int y, Tile* checking, Tile *tile) {
    int tx = x;
    int ty = y;
    // check up
    while(tx > 0){
        tx--;
        checking = boardArr[tx][ty];
        if(checking->getTeam() == tile->getTeam()) {
            return;
        }
        checking->setBlue();
        resetAfter.push_back(checking);
        movePossible.push_back(checking);
        // checking for enemy in the way
        if(checking->getType() != Type::none) {
            return; // enemy here break looping
        }
    }
}

// checking right
void MainWindow::checkRight(int x, int y, Tile* checking, Tile *tile) {
    int tx = x;
    int ty = y;
    // check up
    while(tx < 7){
        tx++;
        checking = boardArr[tx][ty];
        if(checking->getTeam() == tile->getTeam()) {
            return;
        }
        checking->setBlue();
        resetAfter.push_back(checking);
        movePossible.push_back(checking);
        // checking for enemy in the way
        if(checking->getType() != Type::none) {
            return; // enemy here break looping
        }
    }
}

// checking left down
void MainWindow::checkLD(int x, int y, Tile* checking, Tile *tile) {
    int tx = x;
    int ty = y;
    while(tx < 7 && ty > 0) {
        tx++;
        ty--;
        checking = boardArr[tx][ty];
        // checking for teamate in way
        if(checking->getTeam() == tile->getTeam()) {
            return;
        }
        // at this point path is clear continue to check next
        checking->setBlue();
        resetAfter.push_back(checking);
        movePossible.push_back(checking);
        // checking for enemy in way
        if(checking->getType() != Type::none) {
            return;
        }
    }
}

// checking left up
void MainWindow::checkLU(int x, int y, Tile* checking, Tile *tile) {
    int tx = x;
    int ty = y;
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
}

// checking right down
void MainWindow::checkRD(int x, int y, Tile* checking, Tile *tile) {
    int tx = x;
    int ty = y;
    // check right down diagonal
    while(tx < 7 && ty < 7) {
        tx++;
        ty++;
        checking = boardArr[tx][ty];
        // checking for teamate in way
        if(checking->getTeam() == tile->getTeam()) {
            return;
        }
        // at this point path is clear continue to check next
        checking->setBlue();
        resetAfter.push_back(checking);
        movePossible.push_back(checking);
        // checking for enemy in way
        if(checking->getType() != Type::none) {
            return;
        }
    }
}

// checking right up
void MainWindow::checkRU(int x, int y, Tile* checking, Tile *tile) {
    int tx = x;
    int ty = y;
    // check right up diagonal
    while(tx > 0 && ty < 7) {
        tx--;
        ty++;
        checking = boardArr[tx][ty];
        // checking for teamate in way
        if(checking->getTeam() == tile->getTeam()) {
            return;
        }
        // at this point path is clear continue to check next
        checking->setBlue();
        resetAfter.push_back(checking);
        movePossible.push_back(checking);
        // checking for enemy in way
        if(checking->getType() != Type::none) {
            return;
        }
    }
}

// checking black pawn
void MainWindow::checkBlackPawn(int x, int y, Tile* checking, Tile *tile) {
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
}

// checking white pawn
void MainWindow::checkWhitePawn(int x, int y, Tile* checking, Tile *tile) {
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
}

// checking king
// void MainWindow::checkKing(int x, int y, Tile* checking, Tile *tile) {

// }

// checking knights
void MainWindow::checkKnight(int x, int y, Tile* checking, Tile *tile) {
    int tx = x;
    int ty = y;

    // |- X - X -| top
    // |X - - - X| top middle
    // |- - 0 - -| middle
    // |X - - - X| bottom middle
    // |- X - X -| bottom

    // check top row
    if(ty-2 >= 0) {
        // top row valid
        // check top left
        if(tx-1 >= 0) {
            // top left valid
            checking = boardArr[tx-1][ty-2];

            if(checking->getTeam() != tile->getTeam()) {
                checking->setBlue();
                movePossible.push_back(checking);
                resetAfter.push_back(checking);
            }
        }
        // check top right
        if(tx+1 <= 7) {
            // top right valid
            checking = boardArr[tx+1][ty-2];

            if(checking->getTeam() != tile->getTeam()) {
                checking->setBlue();
                movePossible.push_back(checking);
                resetAfter.push_back(checking);
            }
        }
    }

    // check top middle row
    if(ty-1 >= 0) {
        // top middle row valid
        // check middle top left
        if(tx-2 >= 0) {
            // middle top left valid
            checking = boardArr[tx-2][ty-1];

            if(checking->getTeam() != tile->getTeam()) {
                checking->setBlue();
                movePossible.push_back(checking);
                resetAfter.push_back(checking);
            }
        }
        // check middle top right
        if(tx+2 <= 7) {
            // middle top right valid
            checking = boardArr[tx+2][ty-1];

            if(checking->getTeam() != tile->getTeam()) {
                checking->setBlue();
                movePossible.push_back(checking);
                resetAfter.push_back(checking);
            }
        }
    }

    // check bottom middle row
    if(ty+1 <= 7) {
        // top row valid
        // check top left
        if(tx-2 >= 0) {
            // top left valid
            checking = boardArr[tx-2][ty+1];

            if(checking->getTeam() != tile->getTeam()) {
                checking->setBlue();
                movePossible.push_back(checking);
                resetAfter.push_back(checking);
            }
        }
        // check top right
        if(tx+2 <= 7) {
            // top right valid
            checking = boardArr[tx+2][ty+1];

            if(checking->getTeam() != tile->getTeam()) {
                checking->setBlue();
                movePossible.push_back(checking);
                resetAfter.push_back(checking);
            }
        }
    }

    // check bottom row
    if(ty+2 <= 7) {
        // bottom middle row valid
        // check middle bottom left
        if(tx-1 >= 0) {
            // middle bottom left valid
            checking = boardArr[tx-1][ty+2];

            if(checking->getTeam() != tile->getTeam()) {
                checking->setBlue();
                movePossible.push_back(checking);
                resetAfter.push_back(checking);
            }
        }
        // check middle bottom right
        if(tx+1 <= 7) {
            // middle bottom right valid
            checking = boardArr[tx+1][ty+2];

            if(checking->getTeam() != tile->getTeam()) {
                checking->setBlue();
                movePossible.push_back(checking);
                resetAfter.push_back(checking);
            }
        }
    }
}

// deconstructor
MainWindow::~MainWindow() {}