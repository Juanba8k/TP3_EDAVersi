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

void getValidMoves(GameModel& model, Moves& validMoves) {

    validMoves.clear();

    Player currentPlayer = getCurrentPlayer(model);
    Piece currentPiece = (currentPlayer == PLAYER_WHITE) ? PIECE_WHITE : PIECE_BLACK;
    Piece opponentPiece = (currentPlayer == PLAYER_WHITE) ? PIECE_BLACK : PIECE_WHITE;

    // Possible directions
    const int directions[8][2] = {{0, 1},{1, 1},{1, 0},{1, -1},{0, -1},{-1, -1},{-1, 0},{-1, 1}};

    // Check each square on the board
    for (int y = 0; y < BOARD_SIZE; y++) 
    {
        for (int x = 0; x < BOARD_SIZE; x++) 
        {
            Square square = { x, y };

            // check if square is empty
            if (getBoardPiece(model, square) != PIECE_EMPTY) 
            {
                continue;
            }

            bool isValidMove = false;

            for (int dir = 0; dir < DIRECTIONS; dir++) 
            {
                int dx = directions[dir][0];
                int dy = directions[dir][1];

                // Movee one position in the current direction 
                Square nextSquare = { x + dx, y + dy };

                // Check if the square is valid and contains opponent´s piece
                if (!isSquareValid(nextSquare) || getBoardPiece(model, nextSquare) != opponentPiece) 
                {
                    continue;
                }

                // Continue in that direction looking for your own piece
                int steps = 2;
                while (true) 
                {
                    Square checkSquare = { x + (dx * steps), y + (dy * steps) };

                    if (!isSquareValid(checkSquare) || getBoardPiece(model, checkSquare) == PIECE_EMPTY) 
                    {
                        break;
                    }

                    if (getBoardPiece(model, checkSquare) == currentPiece) 
                    {
                        isValidMove = true;
                        break;
                    }

                    steps++;
                }

                // If we find a valid move, we don´t need to check the other directions
                if (isValidMove) 
                {
                    break;
                }
            }

            // Add valid move to the list
            if (isValidMove) 
            {
                validMoves.push_back(square);
            }
        }
    }
}

bool playMove(GameModel& model, Square move)
{
    Moves validMoves;
    getValidMoves(model, validMoves);

    bool isValid = false;
    for (const Square& validMove : validMoves) 
    {
        if (validMove.x == move.x && validMove.y == move.y) 
        {
            isValid = true;
            break;
        }
    }

    if (!isValid) 
    {
        return false;
    }

    Player currentPlayer = getCurrentPlayer(model);
    Piece currentPiece = (currentPlayer == PLAYER_WHITE) ? PIECE_WHITE : PIECE_BLACK;
    Piece opponentPiece = (currentPlayer == PLAYER_WHITE) ? PIECE_BLACK : PIECE_WHITE;

    setBoardPiece(model, move, currentPiece);

    const int directions[8][2] = {{0, 1},{1, 1},{1, 0},{1, -1},{0, -1},{-1, -1},{-1, 0},{-1, 1}};

    for (int d = 0; d < DIRECTIONS; d++) 
    {
        int dx = directions[d][0];
        int dy = directions[d][1];

        Square nextSquare = { move.x + dx, move.y + dy };

        if (!isSquareValid(nextSquare) || getBoardPiece(model, nextSquare) != opponentPiece) 
        {
            continue;
        }

        int steps = 2;
        bool foundOwnPiece = false;

        while (true) 
        {
            Square checkSquare = { move.x + (dx * steps), move.y + (dy * steps) };

            if (!isSquareValid(checkSquare) || getBoardPiece(model, checkSquare) == PIECE_EMPTY) 
            {
                break;
            }
            if (getBoardPiece(model, checkSquare) == currentPiece) 
            {
                foundOwnPiece = true;
                break;
            }
            steps++;
        }

        // If we found our own piece, flip all opponent's pieces in this direction
        if (foundOwnPiece) 
        {
            for (int i = 1; i < steps; i++) 
            {
                Square flipSquare = { move.x + (dx * i), move.y + (dy * i) };
                setBoardPiece(model, flipSquare, currentPiece);
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
    // Moves validMoves; -> ver xq me tira error ??
    getValidMoves(model, validMoves);

    if (validMoves.size() == 0)
        model.gameOver = true;

    return true;

}