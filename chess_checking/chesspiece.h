#pragma once
/*
 * board.h
 */
#ifndef CHESSPIECE_H_
#define CHESSPIECE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <cctype>

#include "utils.h"

using namespace std;


// NOTE: DO NOT CHANGE `Chesspiece` CLASS EXCEPT THE RETURN TYPE OF `get_possible_moves()`
class ChessPiece
{
public:
    ChessPiece(string, string, int, int, int=0);
    ChessPiece(const ChessPiece& other);
    friend ostream& operator<<(ostream& os, const ChessPiece& piece);

    string get_color() { return color; }
    string get_type() { return type; }

    int get_x() { return x; }
    int get_y() { return y; }
    int get_flag() { return flag; }

    void set_x(int x) { this->x = x; }
    void set_y(int y) { this->y = y; }
    void set_flag(int flag) { this->flag = flag; }

    virtual void move(int x, int y) { this->x = x; this->y = y;};

    // NOTE: YOU CAN MODIFY THE RETURN TYPE OF THE FUNCTION BELOW

    virtual vector<tuple<MoveType, Direction, int>> get_possible_moves() {
        return vector<tuple<MoveType, Direction, int>>(); 
    };    // int = 1 한칸만 이동 가능   int = 2 두칸 이동가능(폰)   int = 3 무한 이동 가능(킹,룩,퀸)


protected:
    string color;
    string type;
    int x;
    int y;
    int flag;
};

ChessPiece* create_piece(string color, string type, int x, int y, int flag=0);

// NOTE: INHERIT ChessPiece CLASS AND IMPLEMENT CLASSES
// NOTE: FILL FREE TO ADD ANY MEMBER VARIABLES OR FUNCTIONS

class King : public ChessPiece
{
    // TODO
public:
    King(string color, int , int, int = 0);

    vector<tuple<MoveType, Direction, int>> get_possible_moves(){
        vector<tuple<MoveType, Direction, int>> possible_move;
        tuple<MoveType, Direction, int> move1 (MoveType::MOVE,Direction::UP, 1);
        tuple<MoveType, Direction, int> move2 (MoveType::MOVE,Direction::DOWN, 1);    
        tuple<MoveType, Direction, int> move3 (MoveType::MOVE,Direction::LEFT, 1);  
        tuple<MoveType, Direction, int> move4 (MoveType::MOVE,Direction::RIGHT, 1);
        tuple<MoveType, Direction, int> move5 (MoveType::MOVE,Direction::UP_LEFT, 1);
        tuple<MoveType, Direction, int> move6 (MoveType::MOVE,Direction::DOWN_LEFT, 1);
        tuple<MoveType, Direction, int> move7 (MoveType::MOVE,Direction::UP_RIGHT, 1);
        tuple<MoveType, Direction, int> move8 (MoveType::MOVE,Direction::DOWN_RIGHT, 1);                          
        possible_move.push_back(move1);
        possible_move.push_back(move2);
        possible_move.push_back(move3);
        possible_move.push_back(move4);
        possible_move.push_back(move5);
        possible_move.push_back(move6);
        possible_move.push_back(move7);
        possible_move.push_back(move8);
        return possible_move;
    }
};

class Queen : public ChessPiece
{
public:
    Queen(string color, int , int, int = 0);

    // TODO
    vector<tuple<MoveType, Direction, int>> get_possible_moves(){
        vector<tuple<MoveType, Direction, int>> possible_move;
        tuple<MoveType, Direction, int> move1 (MoveType::MOVE,Direction::UP, 3);
        tuple<MoveType, Direction, int> move2 (MoveType::MOVE,Direction::DOWN, 3);    
        tuple<MoveType, Direction, int> move3 (MoveType::MOVE,Direction::LEFT, 3);  
        tuple<MoveType, Direction, int> move4 (MoveType::MOVE,Direction::RIGHT, 3);
        tuple<MoveType, Direction, int> move5 (MoveType::MOVE,Direction::UP_LEFT, 3);
        tuple<MoveType, Direction, int> move6 (MoveType::MOVE,Direction::DOWN_LEFT, 3);
        tuple<MoveType, Direction, int> move7 (MoveType::MOVE,Direction::UP_RIGHT, 3);
        tuple<MoveType, Direction, int> move8 (MoveType::MOVE,Direction::DOWN_RIGHT, 3);                          
        possible_move.push_back(move1);
        possible_move.push_back(move2);
        possible_move.push_back(move3);
        possible_move.push_back(move4);
        possible_move.push_back(move4);
        possible_move.push_back(move5);
        possible_move.push_back(move6);
        possible_move.push_back(move7);
        possible_move.push_back(move8);
        return possible_move;
    }

};

class Rook : public ChessPiece
{
public:
    Rook(string color, int , int, int = 0);

    // TODO
    vector<tuple<MoveType, Direction, int>> get_possible_moves(){
        vector<tuple<MoveType, Direction, int>> possible_move;
        tuple<MoveType, Direction, int> move1 (MoveType::MOVE,Direction::UP, 3);
        tuple<MoveType, Direction, int> move2 (MoveType::MOVE,Direction::DOWN, 3);
        tuple<MoveType, Direction, int> move3 (MoveType::MOVE,Direction::LEFT, 3);  
        tuple<MoveType, Direction, int> move4 (MoveType::MOVE,Direction::RIGHT, 3);  
        possible_move.push_back(move1);
        possible_move.push_back(move2);
        possible_move.push_back(move3);
        possible_move.push_back(move4);
        return possible_move;

    }
};

class Bishop : public ChessPiece
{
public:
    Bishop(string color, int , int, int = 0);    

    // TODO
    vector<tuple<MoveType, Direction, int>> get_possible_moves(){
        vector<tuple<MoveType, Direction, int>> possible_move;    
        tuple<MoveType, Direction, int> move5 (MoveType::MOVE,Direction::UP_LEFT, 3);
        tuple<MoveType, Direction, int> move6 (MoveType::MOVE,Direction::DOWN_LEFT, 3);
        tuple<MoveType, Direction, int> move7 (MoveType::MOVE,Direction::UP_RIGHT, 3);
        tuple<MoveType, Direction, int> move8 (MoveType::MOVE,Direction::DOWN_RIGHT, 3);         
        possible_move.push_back(move5);
        possible_move.push_back(move6);
        possible_move.push_back(move7);
        possible_move.push_back(move8);
        return possible_move;
    }        
};

class Knight : public ChessPiece
{
public: 
    Knight(string color, int , int, int = 0);

    // TODO

    vector<tuple<MoveType, Direction, int>> get_possible_moves(){
        vector<tuple<MoveType, Direction, int>> possible_move;
        tuple<MoveType, Direction, int> move1 (MoveType::MOVE,Direction::UP_UP_LEFT, 1);
        tuple<MoveType, Direction, int> move2 (MoveType::MOVE,Direction::UP_UP_RIGHT, 1);    
        tuple<MoveType, Direction, int> move3 (MoveType::MOVE,Direction::DOWN_DOWN_LEFT, 1);  
        tuple<MoveType, Direction, int> move4 (MoveType::MOVE,Direction::DOWN_DOWN_RIGHT, 1);
        tuple<MoveType, Direction, int> move5 (MoveType::MOVE,Direction::LEFT_LEFT_DOWN, 1);
        tuple<MoveType, Direction, int> move6 (MoveType::MOVE,Direction::LEFT_LEFT_UP, 1);
        tuple<MoveType, Direction, int> move7 (MoveType::MOVE,Direction::RIGHT_RIGHT_DOWN, 1);
        tuple<MoveType, Direction, int> move8 (MoveType::MOVE,Direction::RIGHT_RIGHT_UP, 1);                          
        possible_move.push_back(move1);
        possible_move.push_back(move2);
        possible_move.push_back(move3);
        possible_move.push_back(move4);
        possible_move.push_back(move4);
        possible_move.push_back(move5);
        possible_move.push_back(move6);
        possible_move.push_back(move7);
        possible_move.push_back(move8);
        return possible_move;
    }

};

class Pawn : public ChessPiece
{
public: 
    Pawn(string color, int , int, int = 0);

    // TODO
    vector<tuple<MoveType, Direction, int>> get_possible_moves(){
        vector<tuple<MoveType, Direction, int>> possible_move;
        tuple<MoveType, Direction, int> move1 (MoveType::MOVE,Direction::UP, 1);
        tuple<MoveType, Direction, int> move2 (MoveType::MOVE,Direction::UP, 2);
        tuple<MoveType, Direction, int> move3 (MoveType::ATTACK,Direction::UP_LEFT, 1);
        tuple<MoveType, Direction, int> move4 (MoveType::ATTACK,Direction::UP_RIGHT, 1);   
        possible_move.push_back(move1);
        possible_move.push_back(move2);
        possible_move.push_back(move3);
        possible_move.push_back(move4);
        return possible_move;      
    }
};  

// NOTE: YOU CAN ADD OTHER FUNCTIONS HERE

#endif /* CHESSPIECE_H_ */