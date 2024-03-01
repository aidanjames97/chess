#ifndef TILE_H
#define TILE_H

#include <QPushButton>
#include <QMouseEvent>

using namespace std;

enum class Type {
    pawn,
    bishop,
    knight,
    rook,
    queen,
    king,
    none
};

enum class Color {
    green,
    grey
};

enum class Team {
    white,
    black
};

class Tile : public QPushButton {
    Q_OBJECT
public:
    Tile(QWidget *parent = nullptr);
    void setTeam(Team t); // set white or black
    void setGreen(); // set background to green
    void setLoc(pair<int,int> l); // set coords
    void setType(Type t); // set type if piece
    pair<int, int> getLoc(); // get coords
    Type getType(); // get the type of piece is in square
    Team getTeam(); // get the team (w / b) of the piece
    // setting icons
    void setPawn();
    void setRook();
    void setKnight();
    void setBishop();
    void setQueen();
    void setKing();
    void setBlue();
    void removePiece();
    // after click 
    void setYellow();
    void removeColor();
    // possible moves called after click
    // void Tile::possible();
private:
    Type type;
    Color color;
    Team team;
    pair<int, int> loc;
private slots:
    void mousePressEvent(QMouseEvent *e);
signals:
    void leftClick();
};

#endif // TILE_H