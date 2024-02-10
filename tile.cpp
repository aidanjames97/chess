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

// handles mouse clicks
void Tile::mousePressEvent(QMouseEvent *e) {
    if(e->button() == Qt::LeftButton) {
        emit leftClick();
        setYellow();
    }
}

// set background to yellow (indicate click)
void Tile::setYellow() {
    if(type != Type::none) {
        setStyleSheet("background-color: yellow");
    }

}

// remove yellow background (new click)
void Tile::removeYellow() {
    if(color == Color::green) {
        setStyleSheet("background-color: green");
        return;
    }
    setStyleSheet("background-color: grey");
}
