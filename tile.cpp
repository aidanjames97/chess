#include "mainwindow.h"
#include "tile.h"
#include <iostream>

using namespace std;

// constructor
Tile::Tile(QWidget *parent) : QPushButton(parent) {
    color = Color::grey;
    setStyleSheet("background-color: grey");
    type = Type::none;
}

// setting team
void Tile::setTeam(Team t) {
    team = t;
}

// setting green
void Tile::setGreen() {
    color = Color::green;
    setStyleSheet("background-color: green");
}

// setting pawns
void Tile::setPawn() {
    type = Type::pawn;
    if(team == Team::white) {
        setIcon(QIcon(QString("./../../../pieces/wP.svg")));
        return;
    }
    setIcon(QIcon(QString("./../../../pieces/bP.svg")));
}

// setting rooks
void Tile::setRook() {
    type = Type::rook;
    if(team == Team::white) {
        setIcon(QIcon(QString("./../../../pieces/wR.svg")));
        return;
    }
    setIcon(QIcon(QString("./../../../pieces/bR.svg")));
}

// setting knight
void Tile::setKnight() {
    type = Type::knight;
    if(team == Team::white) {
        setIcon(QIcon(QString("./../../../pieces/wN.svg")));
        return;
    }
    setIcon(QIcon(QString("./../../../pieces/bN.svg")));
}

// setting bishops
void Tile::setBishop() {
    type = Type::bishop;
    if(team == Team::white) {
        setIcon(QIcon(QString("./../../../pieces/wB.svg")));
        return;
    }
    setIcon(QIcon(QString("./../../../pieces/bB.svg")));
}

// setting queen
void Tile::setQueen() {
    type = Type::queen;
    if(team == Team::white) {
        setIcon(QIcon(QString("./../../../pieces/wQ.svg")));
        return;
    }
    setIcon(QIcon(QString("./../../../pieces/bQ.svg")));
}

// setting king
void Tile::setKing() {
    type = Type::king;
    if(team == Team::white) {
        setIcon(QIcon(QString("./../../../pieces/wK.svg")));
        return;
    }
    setIcon(QIcon(QString("./../../../pieces/bK.svg")));
}

// setter for location of tile
void Tile::setLoc(pair<int, int> l) {
    loc.first = l.first;
    loc.second = l.second;
}

// getter for location
pair<int, int> Tile::getLoc() {
    return make_pair(loc.first, loc.second);
}

// getter for type of piece
Type Tile::getType() {
    return type;
}

// getter for the team piece is
Team Tile::getTeam() {
    return team;
}

// handles mouse clicks
void Tile::mousePressEvent(QMouseEvent *e) {
    if(e->button() == Qt::LeftButton) {
        emit leftClick();
    }
}

// set background to yellow (indicate click)
void Tile::setYellow() {
    if(type != Type::none) {
        setStyleSheet("background-color: yellow");
    }

}

// set background to blue to indicate possible move
void Tile::setBlue() {
    setStyleSheet("background-color: blue");
}

// remove icon from the tile
void Tile::removePiece() {
    setIcon(QIcon());
    type = Type::none;
}

// remove yellow background (new click)
void Tile::removeColor() {
    if(color == Color::green) {
        setStyleSheet("background-color: green");
        return;
    }
    setStyleSheet("background-color: grey");
}

// possible moves for piece to make
// void Tile::possible() {
//     switch(type) {
//         case Type::pawn:
//             break;
//         case Type::bishop:
//             break;
//         case Type::knight:
//             break;
//         case Type::rook:
//             break;
//         case Type::queen:
//             break;
//         case Type::king:
//             break;
//     }
//     return;
// }