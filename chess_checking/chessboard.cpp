#include "chessboard.h"

using namespace std;


void ChessBoard::initialize_board()
{
    // TODO: implement initialize_board
    tiles = new ChessPiece**[height];
    for (int i = 0; i < height; i++) {
        tiles[i] = new ChessPiece*[width];
        for (int j = 0; j < width; j++) {
            tiles[i][j] = nullptr;
        }
    }

}

ostream& operator<<(ostream& os, const ChessBoard& board)
{
    // TODO: implement operator<<
    for(int i = 0; i < board.height; i++){
        for(int j = 0; j < board.width; j++){
            if(board.tiles[i][j] == nullptr){
                os << '.';
            }
            else{
                os << *board.tiles[i][j];
            }
        }
        os << endl;
    }
    return os;
}

ChessPiece* ChessBoard::operator[](string s)
{
    // TODO: implement operator[]
    int col = s[0] - 'a';

    // 한자리인지 두자리인지 확인
    if (s.length() == 3) { 
        row = height - stoi(s.substr(1, 2));
    } else { 
        row = height - (s[1] - '0');
    }

    return tiles[row][col];
}

ChessBoard& ChessBoard::operator=(const ChessBoard& other)
{
    // TODO: implement operator=
    if(this == &other){
        return *this;
    }
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            delete tiles[i][j];
        }
        delete tiles[i];
    }
    delete tiles;                                                 // 원래 tiles의 포인터들 제거

    height = other.height;
    width = other.width;
    initialize_board();

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(other.tiles[i][j] == nullptr){
                tiles[i][j] = nullptr;
            }
            else{
                tiles[i][j] = new ChessPiece(*other.tiles[i][j]);
            }
        }
    }

    white_pieces = other.white_pieces;
    black_pieces = other.black_pieces;

    return *this;
}

ChessBoard::ChessBoard(const ChessBoard& other)
{
   
    // TODO: implement copy constructor
    height = other.height;
    width = other.width;
    initialize_board();

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(other.tiles[i][j] != nullptr){
                tiles[i][j] = new ChessPiece(*other.tiles[i][j]);
            }
        }
    }

    white_pieces = other.white_pieces;
    black_pieces = other.black_pieces;

}

void ChessBoard::set_tile(ChessPiece tile){
    if (tiles[tile.get_y()][tile.get_x()] != nullptr) {
        delete tiles[tile.get_y()][tile.get_x()]; // 기존 체스말이 있으면 삭제
    }
    tiles[tile.get_y()][tile.get_x()] = new ChessPiece(tile);
}


bool ChessBoard::is_tile(int x, int y){
    if(tiles[y][x] != nullptr){
        return true;
    }
    else{
        return false;
    }
}

ChessPiece ChessBoard::get_tile(int x, int y){
    return *tiles[y][x];
}

void ChessBoard::remove_tile(int x, int y){
    tiles[y][x] = nullptr; 
}

bool ChessBoard::is_check(int x, int y){
    ChessBoard check(width, height);
    ChessPiece pos("Black", "O", 0, 0, 0);                         //왕이 존재한다면 체크인 포지션
    ChessPiece* target;
    vector<tuple<MoveType, Direction, int>> target_move;

    for(int k = 0; k < width * height; k++){
        int h = k / width;
        int j = k % width;
        if(tiles[h][j] != nullptr){                                                 //특정 위치에 타일 존재
            if((*tiles[h][j]).get_color() != (*tiles[y][x]).get_color()){           //타일 색깔이 주어진 왕의 색깔과 다름
                if((*tiles[h][j]).get_type() == "Knight" || (*tiles[h][j]).get_type() == "King"){
                    if((*tiles[h][j]).get_type()=="Knight"){ target = new Knight((*tiles[h][j]).get_color(),j,h,0); }
                    else{ target = new King((*tiles[h][j]).get_color(),j,h,0); }
                    target_move = (*target).get_possible_moves();                                 
                    for(auto i:target_move){                                               //모든 움직임들에 대하여
                        int x = (*target).get_x();
                        int y = (*target).get_y(); 
                        if ((*target).get_color() == "Black"){                                //말의 색이 검정이라면 어떻게 이동할것인지 설정
                            x -= direction_to_pair[get<1>(i)].first;
                            y += direction_to_pair[get<1>(i)].second;
                            if( !(x < 0 || x >= width) && !(y >= height || y < 0) ){       //말이 판을 벗어나지 않는다면
                                if(is_tile(x,y)){                                 //이동 하는곳에 말이 존재한다면
                                    if(get_tile(x,y).get_color() == "White"){     //그리고 그 말의 색이 검정 말과 다른 흰색이라면
                                        pos.set_x(x);
                                        pos.set_y(y);
                                        check.set_tile(pos);                     //그 자리에 attack이라는 x로 표현되는 말 배치하기
                                    }
                                }
                                else{                                                       //그 자리에 아무것도 없다면 move 말 설정
                                    pos.set_x(x);
                                    pos.set_y(y);
                                    check.set_tile(pos);
                                }
                            }
                        }
                        if ((*target).get_color() == "White"){                                //말의 색이 하양이라면 어떻게 이동할것인지 설정
                            x += direction_to_pair[get<1>(i)].first;
                            y -= direction_to_pair[get<1>(i)].second;
                            if( !(x < 0 || x >= width) && !(y >= height || y < 0) ){       //말이 판을 벗어나지 않는다면
                                if(is_tile(x,y)){                                 //이동 하는곳에 말이 존재한다면
                                    if(get_tile(x,y).get_color() == "Black"){     //그리고 그 말의 색이 흰색 말과 다른 검정이라면
                                        pos.set_x(x);
                                        pos.set_y(y);
                                        check.set_tile(pos);                     
                                    }
                                }
                                else{                                                      
                                    pos.set_x(x);
                                    pos.set_y(y);
                                    check.set_tile(pos);
                                }
                            }
                        }
                    }
                }
                else if((*tiles[h][j]).get_type() == "Queen" || ((*tiles[h][j]).get_type() == "Rook" || (*tiles[h][j]).get_type() == "Bishop")){
                    if((*tiles[h][j]).get_type() == "Queen") { target = new Queen((*tiles[h][j]).get_color(),j,h,0); }
                    else if((*tiles[h][j]).get_type() == "Rook"){ target = new Rook((*tiles[h][j]).get_color(),j,h,0); }
                    else{ target = new Bishop((*tiles[h][j]).get_color(),j,h,0); }
                    target_move = (*target).get_possible_moves();
                    for(auto i:target_move){                                               //모든 움직임들에 대하여
                        int x = (*target).get_x();
                        int y = (*target).get_y(); 
                        if ((*target).get_color() == "Black"){                                     //말의 색이 검정이라면 어떻게 이동할것인지 설정
                            while(1){                                                           //while문을 사용해 이동한 자리에 말이 존재하거나, 판을 벗어나면 동작을 멈춤
                                x -= direction_to_pair[get<1>(i)].first;
                                y += direction_to_pair[get<1>(i)].second;
                                if( !(x < 0 || x >= width) && !(y >= height || y < 0) ){       //말이 판을 벗어나지 않는다면
                                    if(is_tile(x,y)){                                 //이동 하는곳에 말이 존재한다면
                                        if(get_tile(x,y).get_color() == "White"){     //그리고 그 말의 색이 검정 말과 다른 흰색이라면
                                            pos.set_x(x);
                                            pos.set_y(y);
                                            check.set_tile(pos);                     //그 자리에 attack이라는 x로 표현되는 말 배치하기
                                        }
                                        break;
                                    }
                                    else{                                                       //그 자리에 아무것도 없다면 move 말 설정
                                        pos.set_x(x);
                                        pos.set_y(y);
                                        check.set_tile(pos);
                                    }
                                }
                                else{ break; }
                            }
                        }
                        if ((*target).get_color() == "White"){                               //말의 색이 하양이라면 어떻게 이동할것인지 설정
                            while(1){                                
                                x += direction_to_pair[get<1>(i)].first;
                                y -= direction_to_pair[get<1>(i)].second;
                                if( !(x < 0 || x >= width) && !(y >= height || y < 0) ){       //말이 판을 벗어나지 않는다면
                                    if(is_tile(x,y)){                                 //이동 하는곳에 말이 존재한다면
                                        if(get_tile(x,y).get_color() == "Black"){     //그리고 그 말의 색이 흰색 말과 다른 검정이라면
                                            pos.set_x(x);
                                            pos.set_y(y);
                                            check.set_tile(pos);                     //그 자리에 attack이라는 x로 표현되는 말 배치하기
                                        }
                                        break;
                                    }
                                    else{                                                       //그 자리에 아무것도 없다면 move 말 설정
                                        pos.set_x(x);
                                        pos.set_y(y);
                                        check.set_tile(pos);
                                    }
                                }
                                else{ break; }
                            }
                        }
                    }
                }
                else if((*tiles[h][j]).get_type() == "Pawn"){
                    target = new Pawn((*tiles[h][j]).get_color(),j,h,1);
                    target_move = (*target).get_possible_moves();                                     //말이 가능한 움직임들을 모두 저장

                    if (((*target).get_y() == 1 && (*target).get_color() == "Black") || ((*target).get_y() == (height-2) && (*target).get_color() == "White")){
                        (*target).set_flag(0);
                    }
                    if((*target).get_flag() == 0){                          
                        for(auto i:target_move){                                               //모든 움직임들에 대하여
                            int x = (*target).get_x();
                            int y = (*target).get_y(); 
                            if ((*target).get_color() == "Black"){                                //말의 색이 검정이라면 어떻게 이동할것인지 설정
                                x -= direction_to_pair[get<1>(i)].first * get<2>(i);
                                y += direction_to_pair[get<1>(i)].second * get<2>(i);
                                if( !(x < 0 || x >= width) && !(y >= height || y < 0) ){       //말이 판을 벗어나지 않는다면
                                    if(get<0>(i) == MoveType::ATTACK){                             //폰의 Move의 타입이 Attack이라면(대각선 이동 상황)
                                        if(is_tile(x,y)){                                 //이동 하는곳에 말이 존재한다면
                                            if(get_tile(x,y).get_color() == "White"){     //그리고 그 말의 색이 검정 말과 다른 흰색이라면
                                                pos.set_x(x);
                                                pos.set_y(y);
                                                check.set_tile(pos);                     //그 자리에 attack이라는 x로 표현되는 말 배치하기
                                            }
                                        }
                                    }
                                    else if(get<0>(i) == MoveType::MOVE){
                                        if(!is_tile(x,y)){
                                            pos.set_x(x);
                                            pos.set_y(y);
                                            check.set_tile(pos);
                                        }
                                    }
                                }
                            }
                            if ((*target).get_color() == "White"){                                //말의 색이 하양이라면 어떻게 이동할것인지 설정
                                x += direction_to_pair[get<1>(i)].first * get<2>(i);
                                y -= direction_to_pair[get<1>(i)].second * get<2>(i);
                                if( !(x < 0 || x >= width) && !(y >= height || y < 0) ){       //말이 판을 벗어나지 않는다면
                                    if(get<0>(i) == MoveType::ATTACK){                             //폰의 Move의 타입이 Attack이라면(대각선 이동 상황)
                                        if(is_tile(x,y)){                                 //이동 하는곳에 말이 존재한다면
                                            if(get_tile(x,y).get_color() == "Black"){     //그리고 그 말의 색이 검정 말과 다른 흰색이라면
                                                pos.set_x(x);
                                                pos.set_y(y);
                                                check.set_tile(pos);                     //그 자리에 attack이라는 x로 표현되는 말 배치하기
                                            }
                                        }
                                    }
                                    else if(get<0>(i) == MoveType::MOVE){
                                        if(!is_tile(x,y)){
                                            pos.set_x(x);
                                            pos.set_y(y);
                                            check.set_tile(pos);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else if((*target).get_flag() == 1){
                        for(auto i:target_move){                                               //모든 움직임들에 대하여
                            int x = (*target).get_x();
                            int y = (*target).get_y();
                            if(get<2>(i) != 2){
                                if ((*target).get_color() == "Black"){                                //말의 색이 검정이라면 어떻게 이동할것인지 설정
                                    x -= direction_to_pair[get<1>(i)].first;
                                    y += direction_to_pair[get<1>(i)].second;
                                    if( !(x < 0 || x >= width) && !(y >= height || y < 0) ){       //말이 판을 벗어나지 않는다면
                                        if(get<0>(i) == MoveType::ATTACK){                             //폰의 Move의 타입이 Attack이라면(대각선 이동 상황)
                                            if(is_tile(x,y)){                                 //이동 하는곳에 말이 존재한다면
                                                if(get_tile(x,y).get_color() == "White"){     //그리고 그 말의 색이 검정 말과 다른 흰색이라면
                                                    pos.set_x(x);
                                                    pos.set_y(y);
                                                    check.set_tile(pos);                     //그 자리에 attack이라는 x로 표현되는 말 배치하기
                                                }
                                            }
                                        }
                                        else if(get<0>(i) == MoveType::MOVE){
                                            if(!is_tile(x,y)){
                                                pos.set_x(x);
                                                pos.set_y(y);
                                                check.set_tile(pos);
                                            }
                                        }
                                    }
                                }
                                if ((*target).get_color() == "White"){                                //말의 색이 하양이라면 어떻게 이동할것인지 설정
                                    x += direction_to_pair[get<1>(i)].first * get<2>(i);
                                    y -= direction_to_pair[get<1>(i)].second * get<2>(i);
                                    if( !(x < 0 || x >= width) && !(y >= height || y < 0) ){       //말이 판을 벗어나지 않는다면
                                        if(get<0>(i) == MoveType::ATTACK){                             //폰의 Move의 타입이 Attack이라면(대각선 이동 상황)
                                            if(is_tile(x,y)){                                 //이동 하는곳에 말이 존재한다면
                                                if(get_tile(x,y).get_color() == "Black"){     //그리고 그 말의 색이 검정 말과 다른 흰색이라면
                                                    pos.set_x(x);
                                                    pos.set_y(y);
                                                    check.set_tile(pos);                     //그 자리에 attack이라는 x로 표현되는 말 배치하기
                                                }
                                            }
                                        }
                                        else if(get<0>(i) == MoveType::MOVE){
                                            if(!is_tile(x,y)){
                                                pos.set_x(x);
                                                pos.set_y(y);
                                                check.set_tile(pos);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

            }
        }
    }   

    if(check.is_tile(x,y)){ return true; }
    else{return false;}                                    

}


bool ChessBoard::is_checkmate(int x, int y){
    if(!is_check(x,y)){
        return false;
    }
    else{
        ChessPiece* King_org;
        King_org = new King((*tiles[y][x]).get_color(), x, y, 0);
        ChessPiece *target;
        target = new King((*tiles[y][x]).get_color(), x, y, 0);
        vector<tuple<MoveType, Direction, int>> target_move;
        target_move = (*target).get_possible_moves();
        pair<int,int> position;
        stack<pair<int,int>> possible_king;                                //king이 이동 가능한 위치들을 모두 저장하는 Stack

        for(auto i:target_move){                                               //모든 움직임들에 대하여
            int a = (*target).get_x();
            int b = (*target).get_y(); 
            if ((*target).get_color() == "Black"){                                //말의 색이 검정이라면 어떻게 이동할것인지 설정
                a -= direction_to_pair[get<1>(i)].first;
                b += direction_to_pair[get<1>(i)].second;
                if( !(a < 0 || a >= width) && !(b >= height || b < 0) ){       //말이 판을 벗어나지 않는다면
                    if(!(is_tile(a,b) && get_tile(a,b).get_color() == "Black")){                                 //이동 하는곳에 말이 존재한다면
                            position = make_pair(a,b);
                            possible_king.push(position);
                    }
                }
            }
            else if ((*target).get_color() == "White"){                                //말의 색이 하양이라면 어떻게 이동할것인지 설정
                a += direction_to_pair[get<1>(i)].first;
                b -= direction_to_pair[get<1>(i)].second;
                if( !(a < 0 || a >= width) && !(b >= height || b < 0) ){       //말이 판을 벗어나지 않는다면
                    if(!(is_tile(a,b) && get_tile(a,b).get_color() == "White")){                                 //이동 하는곳에 말이 존재한다면
                        position = make_pair(a,b);
                        possible_king.push(position);
                    }
                }
            }
        }

        int m,n, g = 0;
        ChessPiece *copy;
        remove_tile(x,y);
        while (!possible_king.empty()) {
            m = possible_king.top().first;
            n = possible_king.top().second;
            (*target).set_x(m);
            (*target).set_y(n);
            if(is_tile(m,n)){
                copy = new ChessPiece(get_tile(m,n));
                g = 1;
            }
            set_tile(*target);
            if (!is_check(m, n)) {
                return false;
            }
            remove_tile(m,n);
            possible_king.pop();
            if( g == 1 ){
                set_tile(*copy);
                g = 0;
            }
        }
        delete target;
        set_tile(*King_org);
        ChessPiece* target_original;
        for(int num = 0; num < width * height ; num++){
            int j = num % width;
            int h = num / width;
            if(is_tile(j,h) && (get_tile(j,h).get_color() == (*tiles[y][x]).get_color() && (*tiles[h][j]).get_type() != "King")){
                if((*tiles[h][j]).get_type() == "Knight" || (*tiles[h][j]).get_type() == "King"){
                    if((*tiles[h][j]).get_type()=="Knight"){ target = new Knight((*tiles[h][j]).get_color(),j,h,0); }
                    else{ target = new King((*tiles[h][j]).get_color(),j,h,0); }
                    target_move = (*target).get_possible_moves();
                    target_original = new ChessPiece(*target);                                 
                    for(auto i:target_move){                                               //모든 움직임들에 대하여
                        remove_tile(j,h);
                        int xx = (*target).get_x();
                        int yy = (*target).get_y(); 
                        if ((*target).get_color() == "Black"){                                //말의 색이 검정이라면 어떻게 이동할것인지 설정
                            xx -= direction_to_pair[get<1>(i)].first;
                            yy += direction_to_pair[get<1>(i)].second;
                            if( !(xx < 0 || xx >= width) && !(yy >= height || yy < 0) ){       //말이 판을 벗어나지 않는다면
                                if(is_tile(xx,yy)){                                 //이동 하는곳에 말이 존재한다면
                                    if(get_tile(xx,yy).get_color() == "White"){     //그리고 그 말의 색이 검정 말과 다른 흰색이라면
                                        (*target).set_x(xx);
                                        (*target).set_y(yy);
                                        set_tile(*target);
                                        if(!is_check(x,y)){
                                            return false;
                                        }
                                        remove_tile(xx,yy);
                                        set_tile(*target_original);
                                    }
                                }
                                else{                                                       //그 자리에 아무것도 없다면 move 말 설정
                                    (*target).set_x(xx);
                                    (*target).set_y(yy);
                                    set_tile(*target);
                                    if(!is_check(x,y)){
                                        return false;
                                    }
                                    remove_tile(xx,yy);
                                    set_tile(*target_original);
                                }
                            }
                        }
                        if ((*target).get_color() == "White"){                                //말의 색이 하양이라면 어떻게 이동할것인지 설정
                            xx += direction_to_pair[get<1>(i)].first;
                            yy -= direction_to_pair[get<1>(i)].second;
                            if( !(xx < 0 || xx >= width) && !(yy >= height || yy < 0) ){       //말이 판을 벗어나지 않는다면
                                if(is_tile(xx,yy)){                                 //이동 하는곳에 말이 존재한다면
                                    if(get_tile(xx,yy).get_color() == "Black"){     //그리고 그 말의 색이 흰색 말과 다른 검정이라면
                                        (*target).set_x(xx);
                                        (*target).set_y(yy);
                                        set_tile(*target);
                                        if(!is_check(x,y)){
                                            return false;
                                        }
                                        remove_tile(xx,yy);
                                        set_tile(*target_original);
                                    }
                                }
                                else{                                                      
                                    (*target).set_x(xx);
                                    (*target).set_y(yy);
                                    set_tile(*target);
                                    if(!is_check(x,y)){
                                        return false;
                                    }
                                    remove_tile(xx,yy);
                                    set_tile(*target_original);
                                }
                            }
                        }
                    }
                }
                else if((*tiles[h][j]).get_type() == "Queen" || ((*tiles[h][j]).get_type() == "Rook" || (*tiles[h][j]).get_type() == "Bishop")){
                    if((*tiles[h][j]).get_type() == "Queen") { target = new Queen((*tiles[h][j]).get_color(),j,h,0); }
                    else if((*tiles[h][j]).get_type() == "Rook"){ target = new Rook((*tiles[h][j]).get_color(),j,h,0); }
                    else{ target = new Bishop((*tiles[h][j]).get_color(),j,h,0); }
                    target_move = (*target).get_possible_moves();
                    target_original = new ChessPiece(*target);
                    for(auto i:target_move){
                        int xx = (*target).get_x();
                        int yy = (*target).get_y(); 
                        if ((*target).get_color() == "Black"){                                     //말의 색이 검정이라면 어떻게 이동할것인지 설정
                            while(1){        
                                remove_tile(j,h);                                                   //while문을 사용해 이동한 자리에 말이 존재하거나, 판을 벗어나면 동작을 멈춤
                                

                                
                                xx -= direction_to_pair[get<1>(i)].first;
                                yy += direction_to_pair[get<1>(i)].second;
                                if( !(xx < 0 || xx >= width) && !(yy >= height || yy < 0) ){       //말이 판을 벗어나지 않는다면
                                    if(is_tile(xx,yy)){                                 //이동 하는곳에 말이 존재한다면
                                        if(get_tile(xx,yy).get_color() == "White"){     //그리고 그 말의 색이 검정 말과 다른 흰색이라면
                                            (*target).set_x(xx);
                                            (*target).set_y(yy);
                                            set_tile(*target);
                                            if(!is_check(x,y)){
                                                return false;
                                            }
                                            remove_tile(xx,yy);
                                            set_tile(*target_original);                    
                                        }
                                        break;
                                    }
                                    else{                                                       //그 자리에 아무것도 없다면 move 말 설정

                                        (*target).set_x(xx);
                                        (*target).set_y(yy);
                                        set_tile(*target);
                                        if(!is_check(x,y)){
                                            return false;
                                        }
                                        remove_tile(xx,yy);
                                        set_tile(*target_original);
                                    }
                                }
                                else{ 
                                    set_tile(*target_original); 
                                    break;
                                }
                            }
                        }
                        if ((*target).get_color() == "White"){                               //말의 색이 하양이라면 어떻게 이동할것인지 설정
                            while(1){
                                remove_tile(j,h);                                
                                xx += direction_to_pair[get<1>(i)].first;
                                yy -= direction_to_pair[get<1>(i)].second;
                                if( !(xx < 0 || xx >= width) && !(yy >= height || yy < 0) ){       //말이 판을 벗어나지 않는다면
                                    if(is_tile(xx,yy)){                                 //이동 하는곳에 말이 존재한다면
                                        if(get_tile(xx,yy).get_color() == "Black"){     //그리고 그 말의 색이 흰색 말과 다른 검정이라면
                                            (*target).set_x(xx);
                                            (*target).set_y(yy);
                                            set_tile(*target);
                                            if(!is_check(x,y)){
                                                return false;
                                            }
                                            remove_tile(xx,yy);
                                            set_tile(*target_original);                   //그 자리에 attack이라는 x로 표현되는 말 배치하기
                                        }
                                        break;
                                    }
                                    else{                                                       //그 자리에 아무것도 없다면 move 말 설정
                                        (*target).set_x(xx);
                                        (*target).set_y(yy);
                                        set_tile(*target);
                                        if(!is_check(x,y)){
                                            return false;
                                        }
                                        remove_tile(xx,yy);
                                        set_tile(*target_original);
                                    }
                                }
                                else{ 
                                    set_tile(*target_original);    
                                    break; 
                                }
                            }
                        }
                    }
                }
                else if((*tiles[h][j]).get_type() == "Pawn"){
                    target = new Pawn((*tiles[h][j]).get_color(),j,h,1);
                    target_move = (*target).get_possible_moves();//말이 가능한 움직임들을 모두 저장

                    if (((*target).get_y() == 1 && (*target).get_color() == "Black") || ((*target).get_y() == (height-2) && (*target).get_color() == "White")){
                        (*target).set_flag(0);
                    }
                    target_original = new ChessPiece(*target);                                     
                    if((*target).get_flag() == 0){                          
                        for(auto i:target_move){                                               //모든 움직임들에 대하여
                            remove_tile(j,h);
                            int xx = (*target).get_x();
                            int yy = (*target).get_y(); 
                            if ((*target).get_color() == "Black"){                                //말의 색이 검정이라면 어떻게 이동할것인지 설정
                                xx -= direction_to_pair[get<1>(i)].first * get<2>(i);
                                yy += direction_to_pair[get<1>(i)].second * get<2>(i);
                                if( !(xx < 0 || xx >= width) && !(yy >= height || yy < 0) ){       //말이 판을 벗어나지 않는다면
                                    if(get<0>(i) == MoveType::ATTACK){                             //폰의 Move의 타입이 Attack이라면(대각선 이동 상황)
                                        if(is_tile(xx,yy)){                                 //이동 하는곳에 말이 존재한다면
                                            if(get_tile(xx,yy).get_color() == "White"){     //그리고 그 말의 색이 검정 말과 다른 흰색이라면
                                                (*target).set_x(xx);
                                                (*target).set_y(yy);
                                                set_tile(*target);
                                                if(!is_check(x,y)){
                                                    return false;
                                                }
                                                remove_tile(xx,yy);
                                                set_tile(*target_original);                 //그 자리에 attack이라는 x로 표현되는 말 배치하기
                                            }
                                        }
                                    }
                                    else if(get<0>(i) == MoveType::MOVE){
                                        if(!is_tile(xx,yy)){
                                            (*target).set_x(xx);
                                            (*target).set_y(yy);
                                            set_tile(*target);
                                            if(!is_check(x,y)){
                                                return false;
                                            }
                                            remove_tile(xx,yy);
                                            set_tile(*target_original);
                                        }
                                    }
                                }
                            }
                            if ((*target).get_color() == "White"){                                //말의 색이 하양이라면 어떻게 이동할것인지 설정
                                xx += direction_to_pair[get<1>(i)].first * get<2>(i);
                                yy -= direction_to_pair[get<1>(i)].second * get<2>(i);
                                if( !(xx < 0 || xx >= width) && !(yy >= height || yy < 0) ){       //말이 판을 벗어나지 않는다면
                                    if(get<0>(i) == MoveType::ATTACK){                             //폰의 Move의 타입이 Attack이라면(대각선 이동 상황)
                                        if(is_tile(xx,yy)){                                 //이동 하는곳에 말이 존재한다면
                                            if(get_tile(xx,yy).get_color() == "Black"){     //그리고 그 말의 색이 검정 말과 다른 흰색이라면
                                                (*target).set_x(xx);
                                                (*target).set_y(yy);
                                                set_tile(*target);
                                                if(!is_check(x,y)){
                                                    return false;
                                                }
                                                remove_tile(xx,yy);
                                                set_tile(*target_original);             //그 자리에 attack이라는 x로 표현되는 말 배치하기
                                            }
                                        }
                                    }
                                    else if(get<0>(i) == MoveType::MOVE){
                                        if(!is_tile(xx,yy)){
                                            (*target).set_x(xx);
                                            (*target).set_y(yy);
                                            set_tile(*target);
                                            if(!is_check(x,y)){
                                                return false;
                                            }
                                            remove_tile(xx,yy);
                                            set_tile(*target_original);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else if((*target).get_flag() == 1){
                        for(auto i:target_move){ 
                            remove_tile(j,h);                                              //모든 움직임들에 대하여
                            int xx = (*target).get_x();
                            int yy = (*target).get_y();
                            if(get<2>(i) != 2){
                                if ((*target).get_color() == "Black"){                                //말의 색이 검정이라면 어떻게 이동할것인지 설정
                                    xx -= direction_to_pair[get<1>(i)].first;
                                    yy += direction_to_pair[get<1>(i)].second;
                                    if( !(xx < 0 || xx >= width) && !(yy >= height || yy < 0) ){       //말이 판을 벗어나지 않는다면
                                        if(get<0>(i) == MoveType::ATTACK){                             //폰의 Move의 타입이 Attack이라면(대각선 이동 상황)
                                            if(is_tile(xx,yy)){                                 //이동 하는곳에 말이 존재한다면
                                                if(get_tile(x,y).get_color() == "White"){     //그리고 그 말의 색이 검정 말과 다른 흰색이라면
                                                    (*target).set_x(xx);
                                                    (*target).set_y(yy);
                                                    set_tile(*target);
                                                    if(!is_check(x,y)){
                                                        return false;
                                                    }
                                                    remove_tile(xx,yy);
                                                    set_tile(*target_original);                   //그 자리에 attack이라는 x로 표현되는 말 배치하기
                                                }
                                            }
                                        }
                                        else if(get<0>(i) == MoveType::MOVE){
                                            if(!is_tile(xx,yy)){
                                                (*target).set_x(xx);
                                                (*target).set_y(yy);
                                                set_tile(*target);
                                                if(!is_check(x,y)){
                                                    return false;
                                                }
                                                remove_tile(xx,yy);
                                                set_tile(*target_original);
                                            }
                                        }
                                    }
                                }
                                if ((*target).get_color() == "White"){                                //말의 색이 하양이라면 어떻게 이동할것인지 설정
                                    xx += direction_to_pair[get<1>(i)].first * get<2>(i);
                                    yy -= direction_to_pair[get<1>(i)].second * get<2>(i);
                                    if( !(xx < 0 || xx >= width) && !(yy >= height || yy < 0) ){       //말이 판을 벗어나지 않는다면
                                        if(get<0>(i) == MoveType::ATTACK){                             //폰의 Move의 타입이 Attack이라면(대각선 이동 상황)
                                            if(is_tile(xx,yy)){                                 //이동 하는곳에 말이 존재한다면
                                                if(get_tile(xx,yy).get_color() == "Black"){     //그리고 그 말의 색이 검정 말과 다른 흰색이라면
                                                    (*target).set_x(xx);
                                                    (*target).set_y(yy);
                                                    set_tile(*target);
                                                    if(!is_check(x,y)){
                                                        return false;
                                                    }
                                                    remove_tile(xx,yy);
                                                    set_tile(*target_original);                   //그 자리에 attack이라는 x로 표현되는 말 배치하기
                                                }
                                            }
                                        }
                                        else if(get<0>(i) == MoveType::MOVE){
                                            if(!is_tile(xx,yy)){
                                                (*target).set_x(xx);
                                                (*target).set_y(yy);
                                                set_tile(*target);
                                                if(!is_check(x,y)){
                                                    return false;
                                                }
                                                remove_tile(xx,yy);
                                                set_tile(*target_original);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return true;
    }  
    
}
// NOTE: YOU CAN ADD MORE FUNCTIONS HERE
