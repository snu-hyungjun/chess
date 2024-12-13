#include "game.h"

using namespace std;


void Game::read_file(string filename)
{
    ifstream ifs(filename);
    if (!ifs)
    {
        cerr << "Error: file open failed" << endl;
        exit(1);
    }
    
    ifs >> problem_id;

    if(problem_id == 0){
        problem_specification = "";
    }
    else{
        ifs >> problem_specification;
    }

    cout << "problem_id: [" << problem_id << "]" << endl;
    cout << "problem_specification: [" << problem_specification << "]" << endl;

    // TODO: CREATE CHESSBOARD
    int height, width;
    ifs >> height;
    ifs >> width;
    ChessBoard ex(width, height);                          // 기본 아무것도 없는 체스판 생성
    
    char tile_distinct;
    for(int j = 0; j < height * width; j++){
        ifs >> tile_distinct;
        int h = j / width;
        int i = j % width;

        if(tile_distinct == 'r'){
            Rook tile("Black", i, h, 0);
            ex.set_tile(tile);
        }
        else if(tile_distinct == 'q'){
            Queen tile("Black", i, h, 0);
            ex.set_tile(tile);
        }
        else if(tile_distinct == 'p'){
            Pawn tile("Black", i, h, 0);
            ex.set_tile(tile);
        }
        else if(tile_distinct == 'k'){
            King tile("Black", i, h, 0);
            ex.set_tile(tile);
        }
        else if(tile_distinct == 'b'){
            Bishop tile("Black", i, h, 0);
            ex.set_tile(tile);
        }
        else if(tile_distinct == 'n'){
            Knight tile("Black", i, h, 0);
            ex.set_tile(tile);
        }
        else if(tile_distinct == 'R'){
            Rook tile("White", i, h, 0);
            ex.set_tile(tile);
        }
        else if(tile_distinct == 'Q'){
            Queen tile("White", i, h, 0);
            ex.set_tile(tile);
        }
        else if(tile_distinct == 'P'){
            Pawn tile("White", i, h, 0);
            ex.set_tile(tile);
        }
        else if(tile_distinct == 'K'){
            King tile("White", i, h, 0);
            ex.set_tile(tile);
        }
        else if(tile_distinct == 'B'){
            Bishop tile("White", i, h, 0);
            ex.set_tile(tile);
        }
        else if(tile_distinct == 'N'){
            Knight tile("White", i, h, 0);
            ex.set_tile(tile);
        }
    }

    board = new ChessBoard(ex);
}

void Game::open_output_file(string filename)
{
    ofs.open(filename);
    if (!ofs)
    {
        cerr << "Error: file open failed" << endl;
        exit(1);
    }
}

void Game::solve()
{
    switch (problem_id)
    {
    case 0:
        solve_print_board();
        break;
    case 1:
        solve_print_possible_moves();
        break;
    case 2:
        solve_check();
        break;
    case 3:
        solve_checkmate();
        break;
    case 4:
        solve_checkmate_in_one_move();
        break;
    default:
        cout << "Invalid problem_id" << endl;
        break;
    }
    return;
}

void Game::solve_print_board()
{
    ofs << *board;
}

void Game::solve_print_possible_moves()
{
    // TODO
    double h = board->get_height();
    double w = board->get_width();
    ChessBoard result_board(w,h);
    string position;
    position = problem_specification.substr(1,problem_specification.length());
    ChessPiece attack("Black", "X", 0, 0, 0);                                      //말을 잡는 위치(x)
    ChessPiece move("Black", "O", 0, 0, 0);                                        //말이 없던 위치(o)
    vector<tuple<MoveType, Direction, int>> target_move;
    ChessPiece *target;
    if((problem_specification[0] == 'N' || problem_specification[0] == 'n') || (problem_specification[0] == 'K' || problem_specification[0] == 'k') ){ 
        //말이 나이트,킹 일때 (항상 한칸만 이동)
        if(problem_specification[0] == 'N' || problem_specification[0] == 'n'){
            target = new Knight((*(*board)[position]).get_color(),(*(*board)[position]).get_x(),(*(*board)[position]).get_y(),0);                                 //말이 가능한 움직임들을 모두 저장
        }
        else{
            target = new King((*(*board)[position]).get_color(),(*(*board)[position]).get_x(),(*(*board)[position]).get_y(),0);                                     //말이 가능한 움직임들을 모두 저장
        } 
        result_board.set_tile(*target);                                                 //target 말은 그대로 출력
        target_move = (*target).get_possible_moves();                                 
        for(auto i:target_move){                                               //모든 움직임들에 대하여
            int x = (*target).get_x();
            int y = (*target).get_y(); 
            if ((*target).get_color() == "Black"){                                //말의 색이 검정이라면 어떻게 이동할것인지 설정
                x -= direction_to_pair[get<1>(i)].first;
                y += direction_to_pair[get<1>(i)].second;
                if( !(x < 0 || x >= w) && !(y >= h || y < 0) ){       //말이 판을 벗어나지 않는다면
                    if((*board).is_tile(x,y)){                                 //이동 하는곳에 말이 존재한다면
                        if((*board).get_tile(x,y).get_color() == "White"){     //그리고 그 말의 색이 검정 말과 다른 흰색이라면
                            attack.set_x(x);
                            attack.set_y(y);
                            result_board.set_tile(attack);                     //그 자리에 attack이라는 x로 표현되는 말 배치하기
                        }
                    }
                    else{                                                       //그 자리에 아무것도 없다면 move 말 설정
                        move.set_x(x);
                        move.set_y(y);
                        result_board.set_tile(move);
                    }
                }
            }
            if ((*target).get_color() == "White"){                                //말의 색이 하양이라면 어떻게 이동할것인지 설정
                x += direction_to_pair[get<1>(i)].first;
                y -= direction_to_pair[get<1>(i)].second;
                if( !(x < 0 || x >= w) && !(y >= h || y < 0) ){       //말이 판을 벗어나지 않는다면
                    if((*board).is_tile(x,y)){                                 //이동 하는곳에 말이 존재한다면
                        if((*board).get_tile(x,y).get_color() == "Black"){     //그리고 그 말의 색이 흰색 말과 다른 검정이라면
                            attack.set_x(x);
                            attack.set_y(y);
                            result_board.set_tile(attack);                     //그 자리에 attack이라는 x로 표현되는 말 배치하기
                        }
                    }
                    else{                                                       //그 자리에 아무것도 없다면 move 말 설정
                        move.set_x(x);
                        move.set_y(y);
                        result_board.set_tile(move);
                    }
                }
            }
        }
    }
    else if((problem_specification[0] == 'Q' || problem_specification[0] == 'q') || (problem_specification[0] == 'R' || problem_specification[0] == 'r') || (problem_specification[0] == 'B' || problem_specification[0] == 'b' ))
    //말이 퀸,룩,비숍 일때 (무한으로 이동 가능)
    {                              
        if(problem_specification[0] == 'Q' || problem_specification[0] == 'q'){
            target = new Queen((*(*board)[position]).get_color(),(*(*board)[position]).get_x(),(*(*board)[position]).get_y(),0);                                   //말이 가능한 움직임들을 모두 저장
        }
        else if(problem_specification[0] == 'R' || problem_specification[0] == 'r'){
            target = new Rook((*(*board)[position]).get_color(),(*(*board)[position]).get_x(),(*(*board)[position]).get_y(),0);
        }
        else{
            target = new Bishop((*(*board)[position]).get_color(),(*(*board)[position]).get_x(),(*(*board)[position]).get_y(),0);
        }
        result_board.set_tile(*target);                                                 //target 말은 그대로 출력
        target_move = (*target).get_possible_moves();                                     //말이 가능한 움직임들을 모두 저장

        for(auto i:target_move){                                               //모든 움직임들에 대하여
            int x = (*target).get_x();
            int y = (*target).get_y(); 
            if ((*target).get_color() == "Black"){                                     //말의 색이 검정이라면 어떻게 이동할것인지 설정
                while(1){                                                           //while문을 사용해 이동한 자리에 말이 존재하거나, 판을 벗어나면 동작을 멈춤
                    x -= direction_to_pair[get<1>(i)].first;
                    y += direction_to_pair[get<1>(i)].second;
                    if( !(x < 0 || x >= w) && !(y >= h || y < 0) ){       //말이 판을 벗어나지 않는다면
                        if((*board).is_tile(x,y)){                                 //이동 하는곳에 말이 존재한다면
                            if((*board).get_tile(x,y).get_color() == "White"){     //그리고 그 말의 색이 검정 말과 다른 흰색이라면
                                attack.set_x(x);
                                attack.set_y(y);
                                result_board.set_tile(attack);                     //그 자리에 attack이라는 x로 표현되는 말 배치하기
                            }
                            break;
                        }
                        else{                                                       //그 자리에 아무것도 없다면 move 말 설정
                            move.set_x(x);
                            move.set_y(y);
                            result_board.set_tile(move);
                        }
                    }
                    else{ break; }
                }
            }
            if ((*target).get_color() == "White"){                               //말의 색이 하양이라면 어떻게 이동할것인지 설정
                while(1){                                
                    x += direction_to_pair[get<1>(i)].first;
                    y -= direction_to_pair[get<1>(i)].second;
                    if( !(x < 0 || x >= w) && !(y >= h || y < 0) ){       //말이 판을 벗어나지 않는다면
                        if((*board).is_tile(x,y)){                                 //이동 하는곳에 말이 존재한다면
                            if((*board).get_tile(x,y).get_color() == "Black"){     //그리고 그 말의 색이 흰색 말과 다른 검정이라면
                                attack.set_x(x);
                                attack.set_y(y);
                                result_board.set_tile(attack);                     //그 자리에 attack이라는 x로 표현되는 말 배치하기
                            }
                            break;
                        }
                        else{                                                       //그 자리에 아무것도 없다면 move 말 설정
                            move.set_x(x);
                            move.set_y(y);
                            result_board.set_tile(move);
                        }
                    }
                    else{ break; }
                }
            }
        }
    }
    else if((problem_specification[0] == 'P' || problem_specification[0] == 'p')){       //말이 폰 일때 (특수 상황)  

        target = new Pawn((*(*board)[position]).get_color(),(*(*board)[position]).get_x(),(*(*board)[position]).get_y(),1);
        result_board.set_tile(*target);                                                 //target 말은 그대로 출력
        target_move = (*target).get_possible_moves();                                     //말이 가능한 움직임들을 모두 저장

        if (((*target).get_y() == 1 && (*target).get_color() == "Black") || ((*target).get_y() == (h-2) && (*target).get_color() == "White")){
           (*target).set_flag(0);
        }
        if((*target).get_flag() == 0){                          
            for(auto i:target_move){                                               //모든 움직임들에 대하여
                int x = (*target).get_x();
                int y = (*target).get_y(); 
                if ((*target).get_color() == "Black"){                                //말의 색이 검정이라면 어떻게 이동할것인지 설정
                    x -= direction_to_pair[get<1>(i)].first * get<2>(i);
                    y += direction_to_pair[get<1>(i)].second * get<2>(i);
                    if( !(x < 0 || x >= w) && !(y >= h || y < 0) ){       //말이 판을 벗어나지 않는다면
                        if(get<0>(i) == MoveType::ATTACK){                             //폰의 Move의 타입이 Attack이라면(대각선 이동 상황)
                            if((*board).is_tile(x,y)){                                 //이동 하는곳에 말이 존재한다면
                                if((*board).get_tile(x,y).get_color() == "White"){     //그리고 그 말의 색이 검정 말과 다른 흰색이라면
                                    attack.set_x(x);
                                    attack.set_y(y);
                                    result_board.set_tile(attack);                     //그 자리에 attack이라는 x로 표현되는 말 배치하기
                                }
                            }
                        }
                        else if(get<0>(i) == MoveType::MOVE){
                            if(!(*board).is_tile(x,y)){
                                move.set_x(x);
                                move.set_y(y);
                                result_board.set_tile(move);
                            }
                        }
                    }
                }
                else if ((*target).get_color() == "White"){                                //말의 색이 하양이라면 어떻게 이동할것인지 설정
                    x += direction_to_pair[get<1>(i)].first * get<2>(i);
                    y -= direction_to_pair[get<1>(i)].second * get<2>(i);
                    if( !(x < 0 || x >= w) && !(y >= h || y < 0) ){       //말이 판을 벗어나지 않는다면
                        if(get<0>(i) == MoveType::ATTACK){                             //폰의 Move의 타입이 Attack이라면(대각선 이동 상황)
                            if((*board).is_tile(x,y)){                                 //이동 하는곳에 말이 존재한다면
                                if((*board).get_tile(x,y).get_color() == "Black"){     //그리고 그 말의 색이 검은색이라면
                                    attack.set_x(x);
                                    attack.set_y(y);
                                    result_board.set_tile(attack);                     //그 자리에 attack이라는 x로 표현되는 말 배치하기
                                }
                            }
                        }
                        else if(get<0>(i) == MoveType::MOVE){
                            if(!(*board).is_tile(x,y)){
                                move.set_x(x);
                                move.set_y(y);
                                result_board.set_tile(move);
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
                        if( !(x < 0 || x >= w) && !(y >= h || y < 0) ){       //말이 판을 벗어나지 않는다면
                            if(get<0>(i) == MoveType::ATTACK){                             //폰의 Move의 타입이 Attack이라면(대각선 이동 상황)
                                if((*board).is_tile(x,y)){                                 //이동 하는곳에 말이 존재한다면
                                    if((*board).get_tile(x,y).get_color() == "White"){     //그리고 그 말의 색이 검정 말과 다른 흰색이라면
                                        attack.set_x(x);
                                        attack.set_y(y);
                                        result_board.set_tile(attack);                     //그 자리에 attack이라는 x로 표현되는 말 배치하기
                                    }
                                }
                            }
                            else if(get<0>(i) == MoveType::MOVE){
                                if(!(*board).is_tile(x,y)){
                                    move.set_x(x);
                                    move.set_y(y);
                                    result_board.set_tile(move);
                                }
                            }
                        }
                    }
                    if ((*target).get_color() == "White"){                                //말의 색이 하양이라면 어떻게 이동할것인지 설정
                        x += direction_to_pair[get<1>(i)].first;
                        y -= direction_to_pair[get<1>(i)].second;
                        if( !(x < 0 || x >= w) && !(y >= h || y < 0) ){       //말이 판을 벗어나지 않는다면
                            if(get<0>(i) == MoveType::ATTACK){                             //폰의 Move의 타입이 Attack이라면(대각선 이동 상황)
                                if((*board).is_tile(x,y)){                                 //이동 하는곳에 말이 존재한다면
                                    if((*board).get_tile(x,y).get_color() == "Black"){     //그리고 그 말의 색이 검정 말과 다른 흰색이라면
                                        attack.set_x(x);
                                        attack.set_y(y);
                                        result_board.set_tile(attack);                     //그 자리에 attack이라는 x로 표현되는 말 배치하기
                                    }
                                }
                            }
                            else if(get<0>(i) == MoveType::MOVE){
                                if(!(*board).is_tile(x,y)){
                                    move.set_x(x);
                                    move.set_y(y);
                                    result_board.set_tile(move);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    ofs << result_board;
}


int Game::solve_check()
{
    // TODO
    // EXAMPLE: "Black king is in check"
    //          "White king is not in check"
    int x,y;

    for(int k = 0; k < (*board).get_width() * (*board).get_height(); k++){
        int w = (*board).get_width();
        int h = k / w;
        int j = k % w;
        if((*board).is_tile(j,h)){
            if((*board).get_tile(j,h).get_type() == "King" && (((*board).get_tile(j,h).get_color() == "Black" && problem_specification == "b") || ((*board).get_tile(j,h).get_color() == "White" && problem_specification == "w"))){  
                x = j;
                y = h;
            }
        }
    }
    if((*board).is_check(x,y)){
        if(problem_specification == "b"){ 
            ofs << "Black king is in check";
        }
        else if(problem_specification == "w"){
            ofs << "White king is in check";
        }
    }
    else{
        if(problem_specification == "b"){ 
            ofs << "Black king is not in check";
        }
        else if(problem_specification == "w"){
            ofs << "White king is not in check";
        }
    }
    return 0;
}
 
int Game::solve_checkmate()
{
    // TODO 
    // EXAMPLE: "Black king is checkmated"
    //          "White king is not checkmated"
    int x,y;
    string color;
    for(int k = 0; k < (*board).get_width() * (*board).get_height(); k++){
        int w = (*board).get_width();
        int h = k / w;
        int j = k % w;
        if((*board).is_tile(j,h)){
            if((*board).get_tile(j,h).get_type() == "King" && (((*board).get_tile(j,h).get_color() == "Black" && problem_specification == "b") || ((*board).get_tile(j,h).get_color() == "White" && problem_specification == "w"))){  
                x = j;
                y = h;
                break;
            }
        }
    }

    if((*board).is_checkmate(x,y)){
        if(problem_specification == "b") { ofs << "Black king is checkmated"; }
        else{ ofs << "White king is checkmated"; }
    }
    else{
        if(problem_specification == "b") { ofs << "Black king is not checkmated"; }
        else{ ofs << "White king is not checkmated"; }
    }
    return 0;
}

int Game::solve_checkmate_in_one_move()
{
    // TODO
    // EXAMPLE: "Black king is checkmated in one move"
    //          "White king is not checkmated in one move"
    int h = (*board).get_height();
    int w = (*board).get_width();
    int x,y;
    bool is_checkmated = false;
    ChessBoard original_board((*board));
    ChessBoard possible_board(w,h);
    vector<ChessBoard> possible;                                   //하나의 움직임으로 가능한 보드이 상황들 전체를 stack에 저장
    ChessPiece* target;
    vector<tuple<MoveType, Direction, int>> target_move;
    string color;
    if(problem_specification == "b"){ color = "Black"; }
    else{color = "White";}
    for(int j = 0; j < (*board).get_height() * (*board).get_width(); j++){
        (*board) = original_board;
        y = j / w;
        x = j % w;
        if((*board).is_tile(x,y) && (*board).get_tile(x,y).get_color() != color){
            if((*board).get_tile(x,y).get_type() == "Knight" || (*board).get_tile(x,y).get_type() == "King" ){ 
                //말이 나이트,킹 일때 (항상 한칸만 이동)
                if((*board).get_tile(x,y).get_type() == "Knight" ){
                    target = new Knight(((*board).get_tile(x,y)).get_color(),((*board).get_tile(x,y)).get_x(),((*board).get_tile(x,y)).get_y(),0);                                 //말이 가능한 움직임들을 모두 저장
                }
                else{
                    target = new King(((*board).get_tile(x,y)).get_color(),((*board).get_tile(x,y)).get_x(),((*board).get_tile(x,y)).get_y(),0);                                     //말이 가능한 움직임들을 모두 저장
                }                                                 
                target_move = (*target).get_possible_moves();                                 
                for(auto i:target_move){                                               //모든 움직임들에 대하여
                    (*board).remove_tile(x,y);
                    int x = (*target).get_x();
                    int y = (*target).get_y(); 
                    if ((*target).get_color() == "Black"){                                //말의 색이 검정이라면 어떻게 이동할것인지 설정
                        x -= direction_to_pair[get<1>(i)].first;
                        y += direction_to_pair[get<1>(i)].second;
                        if( !(x < 0 || x >= w) && !(y >= h || y < 0) ){       //말이 판을 벗어나지 않는다면
                            if(!((*board).is_tile(x,y) && (*board).get_tile(x,y).get_color() == "Black")){                                 //이동 하는곳에 말이 존재한다면
                                (*target).set_x(x);
                                (*target).set_y(y);
                                (*board).set_tile(*target);
                                possible_board = (*board);
                                possible.push_back(possible_board);
                            }
                        }
                    }
                    if ((*target).get_color() == "White"){                                //말의 색이 하양이라면 어떻게 이동할것인지 설정
                        x += direction_to_pair[get<1>(i)].first;
                        y -= direction_to_pair[get<1>(i)].second;
                        if( !(x < 0 || x >= w) && !(y >= h || y < 0) ){       //말이 판을 벗어나지 않는다면
                            if((*board).is_tile(x,y)){                                 //이동 하는곳에 말이 존재한다면
                                if(!((*board).is_tile(x,y) && (*board).get_tile(x,y).get_color() == "White")){                                 //이동 하는곳에 말이 존재한다면
                                    (*target).set_x(x);
                                    (*target).set_y(y);
                                    (*board).set_tile(*target);
                                    possible_board = (*board);
                                    possible.push_back(possible_board);
                                }
                            }
                        }
                    }
                }
            }
            else if((*board).get_tile(x,y).get_type() == "Queen" || ((*board).get_tile(x,y).get_type() == "Rook" || (*board).get_tile(x,y).get_type() == "Bishop"))
            //말이 퀸,룩,비숍 일때 (무한으로 이동 가능)
            {                              
                if((*board).get_tile(x,y).get_type() == "Queen"){
                    target = new Queen(((*board).get_tile(x,y)).get_color(),((*board).get_tile(x,y)).get_x(),((*board).get_tile(x,y)).get_y(),0);                                   //말이 가능한 움직임들을 모두 저장
                }
                else if((*board).get_tile(x,y).get_type() == "Rook"){
                    target = new Rook(((*board).get_tile(x,y)).get_color(),((*board).get_tile(x,y)).get_x(),((*board).get_tile(x,y)).get_y(),0);
                }
                else{
                    target = new Bishop(((*board).get_tile(x,y)).get_color(),((*board).get_tile(x,y)).get_x(),((*board).get_tile(x,y)).get_y(),0);
                }

                target_move = (*target).get_possible_moves();                                     //말이 가능한 움직임들을 모두 저장

                for(auto i:target_move){                                               //모든 움직임들에 대하여
                    (*board).remove_tile(x,y);
                    int x = (*target).get_x();
                    int y = (*target).get_y(); 
                    if ((*target).get_color() == "Black"){                                     //말의 색이 검정이라면 어떻게 이동할것인지 설정
                        while(1){
                            (*board).remove_tile(x,y);                                                           //while문을 사용해 이동한 자리에 말이 존재하거나, 판을 벗어나면 동작을 멈춤
                            x -= direction_to_pair[get<1>(i)].first;
                            y += direction_to_pair[get<1>(i)].second;
                            if( !(x < 0 || x >= w) && !(y >= h || y < 0) ){       //말이 판을 벗어나지 않는다면
                                if((*board).is_tile(x,y)){                                 //이동 하는곳에 말이 존재한다면
                                    if((*board).get_tile(x,y).get_color() == "White"){     //그리고 그 말의 색이 검정 말과 다른 흰색이라면
                                        (*target).set_x(x);
                                        (*target).set_y(y);
                                        (*board).set_tile(*target);
                                        possible_board = (*board);
                                        possible.push_back(possible_board);                    //그 자리에 attack이라는 x로 표현되는 말 배치하기
                                    }
                                    break;
                                }
                                else{                                                    
                                    (*target).set_x(x);
                                    (*target).set_y(y);
                                    (*board).set_tile(*target);
                                    possible_board = (*board);
                                    possible.push_back(possible_board);
                                }
                            }
                            else{ break; }
                        }
                    }
                    if ((*target).get_color() == "White"){                               //말의 색이 하양이라면 어떻게 이동할것인지 설정
                        while(1){                                
                            (*board).remove_tile(x,y);                                                           //while문을 사용해 이동한 자리에 말이 존재하거나, 판을 벗어나면 동작을 멈춤
                            x -= direction_to_pair[get<1>(i)].first;
                            y += direction_to_pair[get<1>(i)].second;
                            if( !(x < 0 || x >= w) && !(y >= h || y < 0) ){       //말이 판을 벗어나지 않는다면
                                if((*board).is_tile(x,y)){                                 //이동 하는곳에 말이 존재한다면
                                    if((*board).get_tile(x,y).get_color() == "White"){     //그리고 그 말의 색이 검정 말과 다른 흰색이라면
                                        (*target).set_x(x);
                                        (*target).set_y(y);
                                        (*board).set_tile(*target);
                                        possible_board = (*board);
                                        possible.push_back(possible_board);                    //그 자리에 attack이라는 x로 표현되는 말 배치하기
                                    }
                                    break;
                                }
                                else{                                                       
                                    (*target).set_x(x);
                                    (*target).set_y(y);
                                    (*board).set_tile(*target);
                                    possible_board = (*board);
                                    possible.push_back(possible_board);
                                }
                            }
                            else{ break; }
                        }
                    }
                }
            }
            else if((*board).get_tile(x,y).get_type() == "Pawn"){       //말이 폰 일때 (특수 상황)  

                target = new Pawn(((*board).get_tile(x,y)).get_color(),((*board).get_tile(x,y)).get_x(),((*board).get_tile(x,y)).get_y(),1);
                target_move = (*target).get_possible_moves();                                     //말이 가능한 움직임들을 모두 저장

                if (((*target).get_y() == 1 && (*target).get_color() == "Black") || ((*target).get_y() == (h-2) && (*target).get_color() == "White")){
                (*target).set_flag(0);
                }
                if((*target).get_flag() == 0){                          
                    for(auto i:target_move){                                               //모든 움직임들에 대하여
                        int x = (*target).get_x();
                        int y = (*target).get_y();
                        (*board).remove_tile(x,y); 
                        if ((*target).get_color() == "Black"){                                //말의 색이 검정이라면 어떻게 이동할것인지 설정
                            x -= direction_to_pair[get<1>(i)].first * get<2>(i);
                            y += direction_to_pair[get<1>(i)].second * get<2>(i);
                            if( !(x < 0 || x >= w) && !(y >= h || y < 0) ){       //말이 판을 벗어나지 않는다면
                                if(get<0>(i) == MoveType::ATTACK){                             //폰의 Move의 타입이 Attack이라면(대각선 이동 상황)
                                    if((*board).is_tile(x,y)){                                 //이동 하는곳에 말이 존재한다면
                                        if((*board).get_tile(x,y).get_color() == "White"){     //그리고 그 말의 색이 검정 말과 다른 흰색이라면
                                            (*target).set_x(x);
                                            (*target).set_y(y);
                                            (*board).set_tile(*target);
                                            possible_board = (*board);
                                            possible.push_back(possible_board);                     //그 자리에 attack이라는 x로 표현되는 말 배치하기
                                        }
                                    }
                                }
                                else if(get<0>(i) == MoveType::MOVE){
                                    if(!(*board).is_tile(x,y)){
                                        (*target).set_x(x);
                                        (*target).set_y(y);
                                        (*board).set_tile(*target);
                                        possible_board = (*board);
                                        possible.push_back(possible_board);
                                    }
                                }
                            }
                        }
                        if ((*target).get_color() == "White"){                                //말의 색이 하양이라면 어떻게 이동할것인지 설정
                            x += direction_to_pair[get<1>(i)].first * get<2>(i);
                            y -= direction_to_pair[get<1>(i)].second * get<2>(i);
                            if( !(x < 0 || x >= w) && !(y >= h || y < 0) ){       //말이 판을 벗어나지 않는다면
                                if(get<0>(i) == MoveType::ATTACK){                             //폰의 Move의 타입이 Attack이라면(대각선 이동 상황)
                                    if((*board).is_tile(x,y)){                                 //이동 하는곳에 말이 존재한다면
                                        if((*board).get_tile(x,y).get_color() == "Black"){     //그리고 그 말의 색이 검정 말과 다른 흰색이라면
                                            (*target).set_x(x);
                                            (*target).set_y(y);
                                            (*board).set_tile(*target);
                                            possible_board = (*board);
                                            possible.push_back(possible_board);                     //그 자리에 attack이라는 x로 표현되는 말 배치하기
                                        }
                                    }
                                }
                                else if(get<0>(i) == MoveType::MOVE){
                                    if(!(*board).is_tile(x,y)){
                                        (*target).set_x(x);
                                        (*target).set_y(y);
                                        (*board).set_tile(*target);
                                        possible_board = (*board);
                                        possible.push_back(possible_board);
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
                        (*board).remove_tile(x,y); 
                        if(get<2>(i) != 2){
                            if ((*target).get_color() == "Black"){                                //말의 색이 검정이라면 어떻게 이동할것인지 설정
                                x -= direction_to_pair[get<1>(i)].first;
                                y += direction_to_pair[get<1>(i)].second;
                                if( !(x < 0 || x >= w) && !(y >= h || y < 0) ){       //말이 판을 벗어나지 않는다면
                                    if(get<0>(i) == MoveType::ATTACK){                             //폰의 Move의 타입이 Attack이라면(대각선 이동 상황)
                                        if((*board).is_tile(x,y)){                                 //이동 하는곳에 말이 존재한다면
                                            if((*board).get_tile(x,y).get_color() == "White"){     //그리고 그 말의 색이 검정 말과 다른 흰색이라면
                                                (*target).set_x(x);
                                                (*target).set_y(y);
                                                (*board).set_tile(*target);
                                                possible_board = (*board);
                                                possible.push_back(possible_board);                    
                                            }
                                        }
                                    }
                                    else if(get<0>(i) == MoveType::MOVE){
                                        if(!(*board).is_tile(x,y)){
                                            (*target).set_x(x);
                                            (*target).set_y(y);
                                            (*board).set_tile(*target);
                                            possible_board = (*board);
                                            possible.push_back(possible_board); 
                                        }
                                    }
                                }
                            }
                            if ((*target).get_color() == "White"){                                //말의 색이 하양이라면 어떻게 이동할것인지 설정
                                x += direction_to_pair[get<1>(i)].first;
                                y -= direction_to_pair[get<1>(i)].second ;
                                if( !(x < 0 || x >= w) && !(y >= h || y < 0) ){       //말이 판을 벗어나지 않는다면
                                    if(get<0>(i) == MoveType::ATTACK){                             //폰의 Move의 타입이 Attack이라면(대각선 이동 상황)
                                        if((*board).is_tile(x,y)){                                 //이동 하는곳에 말이 존재한다면
                                            if((*board).get_tile(x,y).get_color() == "White"){     //그리고 그 말의 색이 검정 말과 다른 흰색이라면
                                                (*target).set_x(x);
                                                (*target).set_y(y);
                                                (*board).set_tile(*target);
                                                possible_board = (*board);
                                                possible.push_back(possible_board);                     //그 자리에 attack이라는 x로 표현되는 말 배치하기
                                            }
                                        }
                                    }
                                    else if(get<0>(i) == MoveType::MOVE){
                                        if(!(*board).is_tile(x,y)){
                                            (*target).set_x(x);
                                            (*target).set_y(y);
                                            (*board).set_tile(*target);
                                            possible_board = (*board);
                                            possible.push_back(possible_board);
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

    int k_x, k_y;
    (*board) = original_board;
    for(int k = 0; k < (*board).get_width() * (*board).get_height(); k++){
        int ww = (*board).get_width();
        int hh = k / w;
        int jj = k % w;
        if((*board).is_tile(jj,hh)){
            if((*board).get_tile(jj,hh).get_type() == "King" && (((*board).get_tile(jj,hh).get_color() == "Black" && problem_specification == "b") || ((*board).get_tile(jj,hh).get_color() == "White" && problem_specification == "w"))){  
                k_x = jj;
                k_y = hh;
                break;
            }
        }
    }


    for(ChessBoard bb: possible){
        if(bb.is_checkmate(k_x, k_y)){
            is_checkmated = true;
            break;
        }
    }             

    if(is_checkmated){
        if(problem_specification == "b"){ ofs << "Black king is checkmated in one move"; }
        else{ ofs << "White king is checkmated in one move"; }
    }
    else{ 
        if(problem_specification == "b"){ ofs << "Black king is not checkmated in one move"; }
        else{ ofs << "White king is not checkmated in one move"; }   
    }
    return 0;
}

