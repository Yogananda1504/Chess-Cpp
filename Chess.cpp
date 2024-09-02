#include <iostream>
#include <stdlib.h>
using namespace std;

class GamePiece
{
public:
    GamePiece(char pieceColor) : pieceColor(pieceColor) {}
    virtual ~GamePiece() {}
    virtual char GetPiece() = 0;
    char GetColor()
    {
        return pieceColor;
    }
    bool IsLegalMove(int srcRow, int srcCol, int destRow, int destCol, GamePiece *gameBoard[8][8])
    {
        GamePiece *destPiece = gameBoard[destRow][destCol];
        if ((destPiece == 0) || (pieceColor != destPiece->GetColor()))
        {
            return AreSquaresLegal(srcRow, srcCol, destRow, destCol, gameBoard);
        }
        return false;
    }

private:
    virtual bool AreSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, GamePiece *gameBoard[8][8]) = 0;
    char pieceColor;
};

class PawnPiece : public GamePiece
{
public:
    PawnPiece(char pieceColor) : GamePiece(pieceColor) {}
    ~PawnPiece() {}

private:
    virtual char GetPiece()
    {
        return 'P';
    }
    bool AreSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, GamePiece *gameBoard[8][8])
    {
        GamePiece *destPiece = gameBoard[destRow][destCol];
        if (destPiece == 0)
        {
            // Destination square is unoccupied
            if (srcCol == destCol)
            {
                if (GetColor() == 'W')
                {
                    if (destRow == srcRow + 1)
                    {
                        return true;
                    }
                }
                else
                {
                    if (destRow == srcRow - 1)
                    {
                        return true;
                    }
                }
            }
        }
        else
        {
            // Dest holds piece of opposite color
            if ((srcCol == destCol + 1) || (srcCol == destCol - 1))
            {
                if (GetColor() == 'W')
                {
                    if (destRow == srcRow + 1)
                    {
                        return true;
                    }
                }
                else
                {
                    if (destRow == srcRow - 1)
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }
};

class KnightPiece : public GamePiece
{
public:
    KnightPiece(char pieceColor) : GamePiece(pieceColor) {}
    ~KnightPiece() {}

private:
    virtual char GetPiece()
    {
        return 'N';
    }
    bool AreSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, GamePiece *gameBoard[8][8])
   {
    GamePiece *destPiece = gameBoard[destRow][destCol];
    
    // Knight moves in an L-shape: Check both movement conditions
    bool isLShape =
        ((abs(srcCol - destCol) == 1) && (abs(srcRow - destRow) == 2)) ||
        ((abs(srcCol - destCol) == 2) && (abs(srcRow - destRow) == 1));

    if (!isLShape)
    {
        return false;
    }

    // Check if the destination square is unoccupied or occupied by an opponent's piece
    if (destPiece == nullptr || destPiece->GetColor() != this->GetColor())
    {
        return true;
    }
    
    return false;
    }
};

class BishopPiece : public GamePiece
{
public:
    BishopPiece(char pieceColor) : GamePiece(pieceColor) {}
    ~BishopPiece() {}

private:
    virtual char GetPiece()
    {
        return 'B';
    }
    bool AreSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, GamePiece *gameBoard[8][8])
    {
        if ((destCol - srcCol == destRow - srcRow) || (destCol - srcCol == srcRow - destRow))
        {
            // Make sure that all intervening squares are empty
            int rowOffset = (destRow - srcRow > 0) ? 1 : -1;
            int colOffset = (destCol - srcCol > 0) ? 1 : -1;
            int checkRow;
            int checkCol;
            for (checkRow = srcRow + rowOffset, checkCol = srcCol + colOffset;
                 checkRow != destRow;
                 checkRow = checkRow + rowOffset, checkCol = checkCol + colOffset)
            {
                if (gameBoard[checkRow][checkCol] != 0)
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
};

class RookPiece : public GamePiece
{
public:
    RookPiece(char pieceColor) : GamePiece(pieceColor) {}
    ~RookPiece() {}

private:
    virtual char GetPiece()
    {
        return 'R';
    }
    bool AreSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, GamePiece *gameBoard[8][8])
    {
        if (srcRow == destRow)
        {
            // Make sure that all intervening squares are empty
            int colOffset = (destCol - srcCol > 0) ? 1 : -1;
            for (int checkCol = srcCol + colOffset; checkCol != destCol; checkCol = checkCol + colOffset)
            {
                if (gameBoard[srcRow][checkCol] != 0)
                {
                    return false;
                }
            }
            return true;
        }
        else if (destCol == srcCol)
        {
            // Make sure that all intervening squares are empty
            int rowOffset = (destRow - srcRow > 0) ? 1 : -1;
            for (int checkRow = srcRow + rowOffset; checkRow != destRow; checkRow = checkRow + rowOffset)
            {
                if (gameBoard[checkRow][srcCol] != 0)
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
};

class QueenPiece : public GamePiece
{
public:
    QueenPiece(char pieceColor) : GamePiece(pieceColor) {}
    ~QueenPiece() {}

private:
    virtual char GetPiece()
    {
        return 'Q';
    }
    bool AreSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, GamePiece *gameBoard[8][8])
    {
        if (srcRow == destRow)
        {
            // Make sure that all intervening squares are empty
            int colOffset = (destCol - srcCol > 0) ? 1 : -1;
            for (int checkCol = srcCol + colOffset; checkCol != destCol; checkCol = checkCol + colOffset)
            {
                if (gameBoard[srcRow][checkCol] != 0)
                {
                    return false;
                }
            }
            return true;
        }
        else if (destCol == srcCol)
        {
            // Make sure that all intervening squares are empty
            int rowOffset = (destRow - srcRow > 0) ? 1 : -1;
            for (int checkRow = srcRow + rowOffset; checkRow != destRow; checkRow = checkRow + rowOffset)
            {
                if (gameBoard[checkRow][srcCol] != 0)
                {
                    return false;
                }
            }
            return true;
        }
        else if ((destCol - srcCol == destRow - srcRow) || (destCol - srcCol == srcRow - destRow))
        {
            // Make sure that all intervening squares are empty
            int rowOffset = (destRow - srcRow > 0) ? 1 : -1;
            int colOffset = (destCol - srcCol > 0) ? 1 : -1;
            int checkRow;
            int checkCol;
            for (checkRow = srcRow + rowOffset, checkCol = srcCol + colOffset;
                 checkRow != destRow;
                 checkRow = checkRow + rowOffset, checkCol = checkCol + colOffset)
            {
                if (gameBoard[checkRow][checkCol] != 0)
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
};

class KingPiece : public GamePiece
{
public:
    KingPiece(char pieceColor) : GamePiece(pieceColor) {}
    ~KingPiece() {}

private:
    virtual char GetPiece()
    {
        return 'K';
    }
    bool AreSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, GamePiece *gameBoard[8][8])
    {
        int rowDelta = destRow - srcRow;
        int colDelta = destCol - srcCol;
        if (((rowDelta >= -1) && (rowDelta <= 1)) &&
            ((colDelta >= -1) && (colDelta <= 1)))
        {
            return true;
        }
        return false;
    }
};

class ChessBoard
{
public:
    ChessBoard()
    {
        for (int row = 0; row < 8; ++row)
        {
            for (int col = 0; col < 8; ++col)
            {
                mainGameBoard[row][col] = 0;
            }
        }
        // Allocate and place black pieces
        for (int col = 0; col < 8; ++col)
        {
            mainGameBoard[6][col] = new PawnPiece('B');
        }
        mainGameBoard[7][0] = new RookPiece('B');
        mainGameBoard[7][1] = new KnightPiece('B');
        mainGameBoard[7][2] = new BishopPiece('B');
        mainGameBoard[7][3] = new KingPiece('B');
        mainGameBoard[7][4] = new QueenPiece('B');
        mainGameBoard[7][5] = new BishopPiece('B');
        mainGameBoard[7][6] = new KnightPiece('B');
        mainGameBoard[7][7] = new RookPiece('B');
        // Allocate and place white pieces
        for (int col = 0; col < 8; ++col)
        {
            mainGameBoard[1][col] = new PawnPiece('W');
        }
        mainGameBoard[0][0] = new RookPiece('W');
        mainGameBoard[0][1] = new KnightPiece('W');
        mainGameBoard[0][2] = new BishopPiece('W');
        mainGameBoard[0][3] = new KingPiece('W');
        mainGameBoard[0][4] = new QueenPiece('W');
        mainGameBoard[0][5] = new BishopPiece('W');
        mainGameBoard[0][6] = new KnightPiece('W');
        mainGameBoard[0][7] = new RookPiece('W');
    }
    ~ChessBoard()
    {
        for (int row = 0; row < 8; ++row)
        {
            for (int col = 0; col < 8; ++col)
            {
                delete mainGameBoard[row][col];
                mainGameBoard[row][col] = 0;
            }
        }
    }

    void Print()
    {
        const int squareWidth = 4;
        const int squareHeight = 3;
        for (int row = 0; row < 8 * squareHeight; ++row)
        {
            int squareRow = row / squareHeight;
            // Print side border with numbering
            if (row % 3 == 1)
            {
                cout << '-' << (char)('1' + 7 - squareRow) << '-';
            }
            else
            {
                cout << "---";
            }
            // Print the chess board
            for (int col = 0; col < 8 * squareWidth; ++col)
            {
                int squareCol = col / squareWidth;
                if (((row % 3) == 1) && ((col % 4) == 1 || (col % 4) == 2) && mainGameBoard[7 - squareRow][squareCol] != 0)
                {
                    if ((col % 4) == 1)
                    {
                        cout << mainGameBoard[7 - squareRow][squareCol]->GetColor();
                    }
                    else
                    {
                        cout << mainGameBoard[7 - squareRow][squareCol]->GetPiece();
                    }
                }
                else
                {
                    if ((squareRow + squareCol) % 2 == 1)
                    {
                        cout << '*';
                    }
                    else
                    {
                        cout << ' ';
                    }
                }
            }
            cout << endl;
        }
        // Print the bottom border with numbers
        for (int row = 0; row < squareHeight; ++row)
        {
            if (row % 3 == 1)
            {
                cout << "---";
                for (int col = 0; col < 8 * squareWidth; ++col)
                {
                    int squareCol = col / squareWidth;
                    if ((col % 4) == 1)
                    {
                        cout << (squareCol + 1);
                    }
                    else
                    {
                        cout << '-';
                    }
                }
                cout << endl;
            }
            else
            {
                for (int col = 1; col < 9 * squareWidth; ++col)
                {
                    cout << '-';
                }
                cout << endl;
            }
        }
    }

    bool IsInCheck(char pieceColor)
    {
        // Find the king
        int kingRow;
        int kingCol;
        for (int row = 0; row < 8; ++row)
        {
            for (int col = 0; col < 8; ++col)
            {
                if (mainGameBoard[row][col] != 0)
                {
                    if (mainGameBoard[row][col]->GetColor() == pieceColor)
                    {
                        if (mainGameBoard[row][col]->GetPiece() == 'K')
                        {
                            kingRow = row;
                            kingCol = col;
                        }
                    }
                }
            }
        }
        // Run through the opponent's pieces and see if any can take the king
        for (int row = 0; row < 8; ++row)
        {
            for (int col = 0; col < 8; ++col)
            {
                if (mainGameBoard[row][col] != 0)
                {
                    if (mainGameBoard[row][col]->GetColor() != pieceColor)
                    {
                        if (mainGameBoard[row][col]->IsLegalMove(row, col, kingRow, kingCol, mainGameBoard))
                        {
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }

    bool CanMove(char pieceColor)
    {
        // Run through all pieces
        for (int row = 0; row < 8; ++row)
        {
            for (int col = 0; col < 8; ++col)
            {
                if (mainGameBoard[row][col] != 0)
                {
                    // If it is a piece of the current player, see if it has a legal move
                    if (mainGameBoard[row][col]->GetColor() == pieceColor)
                    {
                        for (int moveRow = 0; moveRow < 8; ++moveRow)
                        {
                            for (int moveCol = 0; moveCol < 8; ++moveCol)
                            {
                                if (mainGameBoard[row][col]->IsLegalMove(row, col, moveRow, moveCol, mainGameBoard))
                                {
                                    // Make move and check whether king is in check
                                    GamePiece *tempPiece = mainGameBoard[moveRow][moveCol];
                                    mainGameBoard[moveRow][moveCol] = mainGameBoard[row][col];
                                    mainGameBoard[row][col] = 0;
                                    bool canMove = !IsInCheck(pieceColor);
                                    // Undo the move
                                    mainGameBoard[row][col] = mainGameBoard[moveRow][moveCol];
                                    mainGameBoard[moveRow][moveCol] = tempPiece;
                                    if (canMove)
                                    {
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return false;
    }

    GamePiece *mainGameBoard[8][8];
};

class ChessGame
{
public:
    ChessGame() : playerTurn('W') {}
    ~ChessGame() {}

    void Start()
    {
        do
        {
            GetNextMove(gameBoard.mainGameBoard);
            AlternateTurn();
        } while (!IsGameOver());
        gameBoard.Print();
    }

    void GetNextMove(GamePiece *gameBoard[8][8])
    {
        bool validMove = false;
        do
        {
            system("clear");
            cout << endl
                 << endl
                 << "          Welcome to Chess Game" << endl
                 << endl
                 << endl;
            cout << "                      Keys to symbols used " << endl
                 << endl
                 << endl;
            cout << " * = white space" << endl;
            cout << " Blank space = black space" << endl;
            cout << " WP = White pawn &  BP = Black pawn" << endl;
            cout << " WN = White Knight & BN = Black Knight" << endl;
            cout << " WB = White Bishop & BB = Black Bishop" << endl;
            cout << " WR = White Rook & BR = Black Rook" << endl;
            cout << " WQ = White Queen & BQ = Black Queen" << endl;
            cout << " WK = White King & BK =Black King" << endl;
            cout << "Rule for move is :" << endl;
            cout << "Move by selecting row & column to another valid location using row & column" << endl
                 << endl
                 << endl;
            this->gameBoard.Print();

            // Get input and convert to coordinates
            cout << playerTurn << "'s Move: ";
            int startMove;
            cin >> startMove;
            int startRow = (startMove / 10) - 1;
            int startCol = (startMove % 10) - 1;

            cout << "To: ";
            int endMove;
            cin >> endMove;
            int endRow = (endMove / 10) - 1;
            int endCol = (endMove % 10) - 1;

            // Check that the indices are in range
            // and that the source and destination are different
            if ((startRow >= 0 && startRow <= 7) &&
                (startCol >= 0 && startCol <= 7) &&
                (endRow >= 0 && endRow <= 7) &&
                (endCol >= 0 && endCol <= 7))
            {
                // Additional checks in here
                GamePiece *currentPiece = gameBoard[startRow][startCol];
                // Check that the piece is the correct color
                if ((currentPiece != 0) && (currentPiece->GetColor() == playerTurn))
                {
                    // Check that the destination is a valid destination
                    if (currentPiece->IsLegalMove(startRow, startCol, endRow, endCol, gameBoard))
                    {
                        // Make the move
                        GamePiece *tempPiece = gameBoard[endRow][endCol];
                        gameBoard[endRow][endCol] = gameBoard[startRow][startCol];
                        gameBoard[startRow][startCol] = 0;
                        // Make sure that the current player is not in check
                        if (!this->gameBoard.IsInCheck(playerTurn))
                        {
                            delete tempPiece;
                            validMove = true;
                        }
                        else
                        { // Undo the last move
                            gameBoard[startRow][startCol] = gameBoard[endRow][endCol];
                            gameBoard[endRow][endCol] = tempPiece;
                        }
                    }
                }
            }
            if (!validMove)
            {
                cout << "Invalid Move!" << endl;
            }
        } while (!validMove);
    }

    void AlternateTurn()
    {
        playerTurn = (playerTurn == 'W') ? 'B' : 'W';
    }

    bool IsGameOver()
    {
        // Check that the current player can move
        // If not, we have a stalemate or checkmate
        bool canMove = gameBoard.CanMove(playerTurn);
        if (!canMove)
        {
            if (gameBoard.IsInCheck(playerTurn))
            {
                AlternateTurn();
                cout << "Checkmate, " << playerTurn << " Wins!" << endl;
            }
            else
            {
                cout << "Stalemate!" << endl;
            }
        }
        return !canMove;
    }

private:
    ChessBoard gameBoard;
    char playerTurn;
};

int main()
{
    ChessGame game;
    game.Start();
    return 0;
}
