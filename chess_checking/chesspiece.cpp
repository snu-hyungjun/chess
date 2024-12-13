#include "chesspiece.h"

using namespace std;


ChessPiece::ChessPiece(string color, string type, int x, int y, int flag)
{
    this->color = color;
    this->type = type;
    this->x = x;
    this->y = y;
    this->flag = flag;
}

King::King(string color, int x, int y, int flag) : ChessPiece(color, "King", x, y, flag){

}


Queen::Queen(string color, int x, int y, int flag) : ChessPiece(color, "Queen", x, y, flag){

}


Rook::Rook(string color, int x, int y, int flag) : ChessPiece(color, "Rook", x, y, flag){

}


Knight::Knight(string color, int x, int y, int flag) : ChessPiece(color, "Knight", x, y, flag){

}


Pawn::Pawn(string color, int x, int y, int flag) : ChessPiece(color, "Pawn", x, y, flag){

}

Bishop::Bishop(string color, int x, int y, int flag) : ChessPiece(color, "Bishop", x, y, flag){

}


ostream& operator<<(ostream& os, const ChessPiece& piece)
{
    char c;
    c = piece_to_char[piece.type];
    if (piece.color == "Black")
    {
        c = tolower(c);
    }
    os << c;
    return os;
}

ChessPiece::ChessPiece(const ChessPiece& other)
{
    this->color = other.color;
    this->type = other.type;
    this->x = other.x;
    this->y = other.y;
    this->flag = other.flag;
}

ChessPiece* create_piece(string color, string type, int x, int y, int flag)
{
    // TODO : implement create_piece
    ChessPiece* a = new ChessPiece(color,type,x,y,flag);
    return a;
}


// NOTE: YOU CAN ADD OTHER FUNCTIONS HERE