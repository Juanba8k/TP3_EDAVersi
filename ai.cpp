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

#define NODE 1000 //veer porque la cota no funciona
#define MAX_DEPTH 10 //tampoco funciona
#define MAX_NODE 500
#define INVALID_SQUARE {-11111,-11111}
#define INF 99999

typedef struct BestMove{
    Square returnMove;
    int actualValue;
};
/**
 * 
 * @brief AlphaBetha algoritm
 * @param node pointer to the node counter
 * @param depth 
 * @param max bool value, if true, it is maximizing
 * @param Board Has the game board with the game state
 * @param move
 * 
 */
BestMove alphaBethaLogic(int* node, int depth, bool max, GameModel Board, Square move, int alfa, int betha);

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
   int node=0;
   BestMove move;
   move = alphaBethaLogic(&node,0,true, model, INVALID_SQUARE, (-INF), (INF));
   printf("total de nodos %d ",node);
   fflush(stdout);

   return move.returnMove;
   /*
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

BestMove alphaBethaLogic(int* node, int depth, bool max, GameModel board, Square move, int alfa, int betha){

    Moves validMoves;
    int actualValue;
    BestMove importantValues ={{0,0},0};
    Square bestMove;

    if (isSquareValid(move)){ //Make sure it is working on valid moves and skips the first move
    playMove(board, move);
    }

    getValidMoves(board, validMoves);

    if (((*node) >= MAX_NODE)||(depth >= MAX_DEPTH)){
        printf("nodos: %d",(*node));
        fflush(stdout);
        importantValues.actualValue = teamDiferentiator(board.board);
        return importantValues;
    }
    if(validMoves.empty()){
        importantValues.actualValue = teamDiferentiator(board.board);
        return importantValues;
    }

    (*node)++;

    if (max){ //tengo que quedarme con el nodo más grande

        for(Square moveMax : validMoves){
            
            importantValues = alphaBethaLogic(node,depth+1,false,board, moveMax, alfa, betha);

            if (importantValues.actualValue>alfa){
                alfa = importantValues.actualValue;
                bestMove = moveMax;
            }
            if (alfa >= betha){
                break;
            }

        }
        importantValues.returnMove = bestMove;
        importantValues.actualValue = alfa;
        return importantValues;
    }
    else{ //tengo que quedarmee con el nodo más chico

        for(Square moveMin : validMoves){
            importantValues = alphaBethaLogic(node,depth+1,true,board, moveMin, alfa, betha);

            if(importantValues.actualValue<betha){
                betha = importantValues.actualValue;
                bestMove = moveMin;
            }
            if (alfa >= betha){
                break;}
        }
        importantValues.returnMove = bestMove;
        importantValues.actualValue=betha; 
        return importantValues;
    }
    return importantValues; //de seguridad
}
