# chess
checking the state of given chessboard
1. 전체 파일은 명령어 혹은 /chess로 컴파일 가능
2. input 파일에
   problem_id
   problem_specification
   (chessboard row col -> row 한칸 띄고 col 입력 (ex)8 8)
   (chessboard)
   을 입력한다
3. problem id 
    0 : print board
    1 : print reachable point
    2 : is it check?
    3 : is it checkmate?
    4 : can it make checkmate in one move?
4. problem specification
   0 : 없음
   1 : position of target chesspiece
   2, 3, 4 : w or b
5. board limitation
   - there should be only one white or black king
   - board size could
