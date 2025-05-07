/**
 * @brief Implements the Reversi game model
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2023-2024
 */

#include "model.h"
#include "raylib.h"
#include <iostream>

#define DIRECTIONS 8

void checkIndex(GameModel& model, Moves& validMoves, Square move, int dx, int dy, int playerPiece);

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

    int playerPiece = (getCurrentPlayer(model) == PLAYER_WHITE) ? PIECE_WHITE : PIECE_BLACK;
    // int countePlayerPiece = (getCurrentPlayer(model)== PLAYER_WHITE )? PIECE_BLACK : PIECE_WHITE;

    const int dx[] = { 1, -1, 0, 0, 1, -1, 1, -1 };
    const int dy[] = { 0, 0, 1, -1, 1, 1, -1, -1 };

    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            Square move = { x, y };
            if (!isSquareValid(move)) {
                continue;
            }
            if (getBoardPiece(model, move) != playerPiece)
            { // me paro diciendo que la pieza no esta vacia y es mia, me interesa que pase enemigo-vacia
                continue;
            }

            for (int dir = 0; dir < DIRECTIONS; dir++)
            {
                checkIndex(model, validMoves, move, dx[dir], dy[dir], playerPiece);
            }
        }
    }
}

/**
 * @brief comentar mejor
 */
void checkIndex(GameModel &model, Moves &validMoves, Square move, int dx, int dy, int playerPiece)
{
    bool contraryFlag = false;
    Square indexMove = move;

    indexMove.x += dx;
    indexMove.y += dy;

    while (isSquareValid(indexMove))
    {
        int piece = getBoardPiece(model, indexMove);
        if (piece == playerPiece)
        {
            break;
        }
        else if (piece == PIECE_EMPTY)
        {
            if (contraryFlag == true)
            {
                validMoves.push_back(indexMove);
            }
            break;
        }
        else{
            contraryFlag = true; 
        }
        indexMove.x += dx;
        indexMove.y += dy;
    }

}

bool playMove(GameModel &model, Square move)
{
    // Set game piece
    Piece piece =
        (getCurrentPlayer(model) == PLAYER_WHITE)
            ? PIECE_WHITE
            : PIECE_BLACK;
    Piece opponentPiece = (piece == PIECE_WHITE) ? PIECE_BLACK : PIECE_WHITE;
    
    setBoardPiece(model, move, piece);
        
    // Flip opponent´s pieces
    // Define possible moves
    const int dx[] = { 1, -1, 0, 0, 1, -1, 1, -1 }; 
    const int dy[] = { 0, 0, 1, -1, 1, 1, -1, -1 }; 

    for (int dir = 0; dir < DIRECTIONS; dir++)
    {
        Square current = move;
        current.x += dx[dir];
        current.y += dy[dir];
        bool opponentBetween = false;

        // Move in the direction while we find opponent's pieces
        while (isSquareValid(current) && getBoardPiece(model, current) == opponentPiece)
        {
            current.x += dx[dir];
            current.y += dy[dir];
            opponentBetween = true;
        }

        // If we find one of our own pieces after passing over at least one opponent's piece, 
        // we go backwards and flip all the opponent's pieces
        if (opponentBetween && isSquareValid(current) && (getBoardPiece(model, current) == piece))
        {
            Square flip = move;
            flip.x += dx[dir];
            flip.y += dy[dir];

            while (isSquareValid(flip) && !(flip.x == current.x && flip.y == current.y))
            {
                setBoardPiece(model, flip, piece);
                flip.x += dx[dir];
                flip.y += dy[dir];
            }
        }
    }


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
