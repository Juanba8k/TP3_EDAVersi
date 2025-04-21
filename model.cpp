/**
 * @brief Implements the Reversi game model
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2023-2024
 */

#include "raylib.h"

#include "model.h"

void initModel(GameModel &model)
{
    model.gameOver = true;

    model.playerTime[0] = 0;
    model.playerTime[1] = 0;

    memset(model.board, PIECE_EMPTY, sizeof(model.board));
}

void startModel(GameModel &model)
{
    model.gameOver = false;

    model.currentPlayer = PLAYER_BLACK;

    model.playerTime[0] = 0;
    model.playerTime[1] = 0;
    model.turnTimer = GetTime();

    memset(model.board, PIECE_EMPTY, sizeof(model.board));
    model.board[BOARD_SIZE / 2 - 1][BOARD_SIZE / 2 - 1] = PIECE_WHITE;
    model.board[BOARD_SIZE / 2 - 1][BOARD_SIZE / 2] = PIECE_BLACK;
    model.board[BOARD_SIZE / 2][BOARD_SIZE / 2] = PIECE_WHITE;
    model.board[BOARD_SIZE / 2][BOARD_SIZE / 2 - 1] = PIECE_BLACK;
}

Player getCurrentPlayer(GameModel &model)
{
    return model.currentPlayer;
}

int getScore(GameModel &model, Player player)
{
    int score = 0;

    for (int y = 0; y < BOARD_SIZE; y++)
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            if (((model.board[y][x] == PIECE_WHITE) &&
                 (player == PLAYER_WHITE)) ||
                ((model.board[y][x] == PIECE_BLACK) &&
                 (player == PLAYER_BLACK)))
                score++;
        }

    return score;
}

double getTimer(GameModel &model, Player player)
{
    double turnTime = 0;

    if (!model.gameOver && (player == model.currentPlayer))
        turnTime = GetTime() - model.turnTimer;

    return model.playerTime[player] + turnTime;
}

Piece getBoardPiece(GameModel &model, Square square)
{
    return model.board[square.y][square.x];
}

void setBoardPiece(GameModel &model, Square square, Piece piece)
{
    model.board[square.y][square.x] = piece;
}

bool isSquareValid(Square square)
{
    return (square.x >= 0) &&
           (square.x < BOARD_SIZE) &&
           (square.y >= 0) &&
           (square.y < BOARD_SIZE);
}

void getValidMoves(GameModel &model, Moves &validMoves)
{
    Piece a;
    // To-do: your code goes here...
    int playerPiece = (getCurrentPlayer(model)== PLAYER_WHITE )? PIECE_WHITE : PIECE_BLACK;
    //int countePlayerPiece = (getCurrentPlayer(model)== PLAYER_WHITE )? PIECE_BLACK : PIECE_WHITE;

    for (int y = 0; y < BOARD_SIZE; y++)
        for (int x = 0; x < BOARD_SIZE; x++) 
        {
            Square move = {x, y};
            
            if (getBoardPiece(model, move) == PIECE_EMPTY)

            for(int y2=0; y2<BOARD_SIZE; y2++){

                int flagmptyY, flagCounterY, flagPlayerPieceY;
                
                for(int x2=0; x2<BOARD_SIZE; x2++){
                    Square indexMove = {x2,y2};
                    int indexPiece = getBoardPiece(model,indexMove);

                    if ((indexMove.y==move.y) && (indexMove.x==move.y)){
                        //hay flag de que habia pieza equipo contrario, mia o blanco antes o ninguna
                        //break o continue?
                    }
                    if (indexPiece==PIECE_EMPTY){
                        //es la primera pieza? -> flagmpty =1;
                        //habian piezas antes
                            //esas eran del oponente, si-> habían antes de las piezas del oponente piezas suyas, si -> valido
                            //                         |                                                         no-> no valida
                            //                         no-> indexMovee==move?, si-> no valido
                            //                                                 no -> hay que ver otros casos, flagmpty =1;

                    }else if(playerPiece!=indexPiece){
                        //es
                    }else if (playerPiece==indexPiece){
                        //habian piezas el oponente antes ?? si -> valido
                        //                                   no -> indexMoove==move? no-> continuo
                        //                                                            si -> movimiento no valido

                    }


                }
            }
            

        }
}

/**
 * a analizar
 * 1) está vacia 
 * 2) al lado tiene una pieza del equipo contrario en x
 *  2.1) al lado tiene una del equipo contrario en Y
 *  2.2) si no tiene, no valido
 */
/* lo del programa antiguo // +++ TEST
                // Lists all empty squares...
                if (getBoardPiece(model, move) == PIECE_EMPTY)
                    validMoves.push_back(move);
                // --- TEST
                */

bool playMove(GameModel &model, Square move)
{
    // Set game piece
    Piece piece =
        (getCurrentPlayer(model) == PLAYER_WHITE)
            ? PIECE_WHITE
            : PIECE_BLACK;

    setBoardPiece(model, move, piece);

    // To-do: your code goes here...

    // Update timer
    double currentTime = GetTime();
    model.playerTime[model.currentPlayer] += currentTime - model.turnTimer;
    model.turnTimer = currentTime;

    // Swap player
    model.currentPlayer =
        (model.currentPlayer == PLAYER_WHITE)
            ? PLAYER_BLACK
            : PLAYER_WHITE;

    // Game over?
    Moves validMoves;
    getValidMoves(model, validMoves);

    if (validMoves.size() == 0)
        model.gameOver = true;

    return true;
}
