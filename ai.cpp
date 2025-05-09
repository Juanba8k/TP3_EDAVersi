/**
 * @brief Implements the Reversi game AI
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2023-2024
 */

#include <cstdlib>

#include "ai.h"
#include "controller.h"
#include <unistd.h>

#define MAX_DEPTH 10
#define MAX_NODE 10000
#define INVALID_SQUARE {-11111, -11111}
#define INF 99999

typedef struct BestMove
{
    Square returnMove;
    int actualValue;
} BestMove;

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
BestMove alphaBethaLogic(int *node, int depth, bool max, GameModel Board, Square move, int alfa, int betha);

/**
 * @cite https://youtu.be/I0y-TGehf-4?si=Y7nc5RNucqB2leI7
 * @cite https://youtu.be/QJjM7EKDRuc?si=zMkmov9msS09ernM
 */

Square getBestMove(GameModel &model)
{

    int node = 0;
    BestMove move;
    move = alphaBethaLogic(&node, 0, true, model, INVALID_SQUARE, (-INF), (INF));

    return move.returnMove;
}

int teamDiferentiator(Piece board[BOARD_SIZE][BOARD_SIZE])
{
    int WhiteCounter = 0;
    int BlackCounter = 0;
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            if (board[y][x] == PIECE_BLACK)
                BlackCounter++;
            if (board[y][x] == PIECE_WHITE)
                WhiteCounter++;
        }
    }
    return WhiteCounter - BlackCounter;
}

BestMove alphaBethaLogic(int *node, int depth, bool max, GameModel board, Square move, int alfa, int betha)
{
    Moves validMoves;
    int actualValue;
    BestMove importantValues = {{0, 0}, 0};
    Square bestMove;

    if (isSquareValid(move))
    { // Make sure it is working on valid moves and skips the first move
        playMove(board, move);
    }

    if (((*node) >= MAX_NODE) || (depth >= MAX_DEPTH))
    { // Final case
        importantValues.actualValue = teamDiferentiator(board.board);
        return importantValues;
    }
    if (validMoves.empty())
    { // Final case
        importantValues.actualValue = teamDiferentiator(board.board);
        return importantValues;
    }
    getValidMoves(board, validMoves);
    (*node)++;

    if (max)
    { // Max mode

        for (Square moveMax : validMoves)
        { // Create child nodes

            importantValues = alphaBethaLogic(node, depth + 1, false, board, moveMax, alfa, betha);

            if (importantValues.actualValue > alfa)
            {
                alfa = importantValues.actualValue;
                bestMove = moveMax;
            }
            if (alfa >= betha)
            { // Poda Alfa Betha
                break;
            }
        }
        importantValues.returnMove = bestMove;
        importantValues.actualValue = alfa;
        return importantValues;
    }
    else
    { // Min mode

        for (Square moveMin : validMoves)
        { // Create child nodes
            importantValues = alphaBethaLogic(node, depth + 1, true, board, moveMin, alfa, betha);

            if (importantValues.actualValue < betha)
            {
                betha = importantValues.actualValue;
                bestMove = moveMin;
            }
            if (alfa >= betha)
            { // Poda Alfa Betha
                break;
            }
        }
        importantValues.returnMove = bestMove;
        importantValues.actualValue = betha;
        return importantValues;
    }
    return importantValues; // Safety return
}
