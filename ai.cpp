/**
 * @brief Implements the Reversi game AI
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2023-2024
 */

#include <cstdlib>

#include "ai.h"
#include "controller.h"
#include<unistd.h>
/*
    class GameState{
        public:
            int teamDiferentiator(){
                int WhiteCounter =0;
                int BlackCounter =0;
                for(int y=0; y<BOARD_SIZE;y++){
                    for(int x=0; x<BOARD_SIZE;x++){
                        if(board[y][x] == PIECE_BLACK) BlackCounter++;
                        if(board[y][x] == PIECE_WHITE) WhiteCounter++;
                    }
                }
                return WhiteCounter-BlackCounter;
            }
            void setter (Piece loadBoard[BOARD_SIZE][BOARD_SIZE]){
                for(int y=0; y<BOARD_SIZE;y++){
                    for(int x=0; x<BOARD_SIZE;x++){
                        board[y][x]=loadBoard[y][x];
                    }
                }
            }
            int alfa;
            int betha;
        private:
            Piece board[BOARD_SIZE][BOARD_SIZE];


    };

*/

#define NODE 500
#define MAX_DEPTH 100
#define MAX_NODE 100

void alphaBethaLogic(int* node, int depth, bool max, GameModel Board, Square move);

Square getBestMove(GameModel &model)
{

    /**
     * @cite https://youtu.be/I0y-TGehf-4?si=Y7nc5RNucqB2leI7
     * @cite https://youtu.be/QJjM7EKDRuc?si=zMkmov9msS09ernM
     * 
     * IMPORTANTEE, SEGUN LA CONVENCION DEFINIDA EN LA CLASE, SI EL NODO ES POSITIVO, HAY MAS PIEZAS BLANCAS, 
     * SI ES NEGATIVO, HAY MAS PIEZAS NEGRAS. SI ES 0, EMPATE.
     */

    //deberia usar getValidMoves
    /*
    int i = 0;
    alphaBethaLogic()
    
    // To-do: your code goes here...
    */
    // +++ TEST
    // Returns a random valid move...
    Moves validMoves;
    getValidMoves(model, validMoves);
    sleep(2);
    int index = rand() % validMoves.size();
    return validMoves[index];
    // --- TEST*/
}

int teamDiferentiator(Piece board[BOARD_SIZE][BOARD_SIZE]){
    int WhiteCounter =0;
    int BlackCounter =0;
    for(int y=0; y<BOARD_SIZE;y++){
        for(int x=0; x<BOARD_SIZE;x++){
            if(board[y][x] == PIECE_BLACK) BlackCounter++;
            if(board[y][x] == PIECE_WHITE) WhiteCounter++;
        }
    }
    return WhiteCounter-BlackCounter;
}
/*
void alphaBethaLogic(int* node, int depth, bool max, GameModel board, Square move){

    Moves validMoves;
    getValidMoves(board, validMoves);
    playMove(board, move);
    (*node)++;

    if (((*node) >= MAX_NODE)||(depth >= MAX_DEPTH)){

        return teamDiferentiator(board.board);
        
    }


    if (max){ //tengo que quedarme con el nodo más grande
        for(Square moveMax : validMoves){

            alphaBethaLogic(node,depth+1,false,board, moveMax);
        }
    }else{ //teengo que quedarmee con el nodo más chico
        for(Square moveMin : validMoves){
            alphaBethaLogic(node,depth+1,true,board, moveMin);
        }

    }
    return; //de seguridad
}*/
