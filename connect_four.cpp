#ifndef CONNECT_FOUR_CPP
#define CONNECT_FOUR_CPP

#include <iostream>
#include <vector>
#include <limits>

class ConnectFour
{
private:
    static const int BOARD_SIZE = 8;
    const int _connectionLength = 4;

public:
    typedef int Move;
    enum Piece
    {
        EMPTY,
        PLAYER1,
        PLAYER2,
    };

    ConnectFour()
        : _board(),
          _turn(true),
          _isGameOver(false) {}

    int
    boardSize() const
    {
        return BOARD_SIZE;
    }

    bool
    isYourTurn() const
    {
        return _turn;
    }

    bool
    isGameOver() const
    {
        return _isGameOver;
    }

    Piece pieceToPlay() const
    {
        return _turn ? Piece::PLAYER1 : Piece::PLAYER2;
    }

    Piece
    at(int x, int y) const
    {
        return _board[x][y];
    }

    void
    makeMove(Move column)
    {
        if (_isGameOver)
            return;

        auto freeRow = getFirstFreeRow(column);
        if (freeRow != -1)
        {
            auto piece = pieceToPlay();
            _board[freeRow][column] = piece;
            _turn = !_turn;
            _isGameOver = checkConnectFour(piece) || isBoardFull();
        }
    }

    std::vector<Move>
    getPossibleMoves() const
    {
        std::vector<Move> possibleColumns;
        possibleColumns.reserve(BOARD_SIZE - 2);

        for (unsigned int i = 1; i < BOARD_SIZE - 1; i++)
        {
            auto freeRow = getFirstFreeRow(i);
            if (freeRow != -1)
                possibleColumns.push_back(i);
        }

        return possibleColumns;
    }

    bool
    checkConnectFour(Piece piece) const
    {
        return checkConnectedPieces(piece, _connectionLength);
    }

    bool
    isBoardFull() const
    {
        for (unsigned int i = 1; i < BOARD_SIZE - 1; i++)
            for (unsigned int j = 1; j < BOARD_SIZE - 1; j++)
                if (_board[i][j] == Piece::EMPTY)
                    return false;

        return true;
    }

    int
    utility() const
    {
        if (checkConnectFour(Piece::PLAYER1))
            return std::numeric_limits<int>::max();

        if (checkConnectFour(Piece::PLAYER2))
            return std::numeric_limits<int>::min();

        return 0;
    }

    int
    evaluate() const
    {
        int valuePlayer1 = 0;
        int valuePlayer2 = 0;

        for (unsigned int i = 1; i < _connectionLength; i++)
        {
            valuePlayer1 +=
                checkConnectedPieces(Piece::PLAYER1, i) * 10 * i;

            valuePlayer2 +=
                checkConnectedPieces(Piece::PLAYER2, i) * 10 * i;
        }

        return valuePlayer1 - valuePlayer2;
    }

    void
    prettyPrint() const
    {
        for (unsigned int i = 0; i < BOARD_SIZE; i++)
        {
            for (unsigned int j = 0; j < BOARD_SIZE; j++)
            {
                switch (_board[i][j])
                {
                case Piece::EMPTY:
                    std::cout << "#"
                              << "|";
                    break;
                case Piece::PLAYER1:
                    std::cout << "x"
                              << "|";
                    break;
                case Piece::PLAYER2:
                    std::cout << "o"
                              << "|";
                    break;

                default:
                    break;
                }
            }
            std::cout << std::endl;
        }
    }

    void
    reset()
    {
        _isGameOver = false;
        _turn = true;
        for (unsigned int i = 1; i < BOARD_SIZE - 1; i++)
            for (unsigned int j = 1; j < BOARD_SIZE - 1; j++)
                _board[i][j] = Piece::EMPTY;
    }

private:
    Piece _board[BOARD_SIZE][BOARD_SIZE];
    bool _isGameOver;
    bool _turn;

    bool
    checkConnectedPieces(Piece piece, int connectionLength) const
    {
        int rowAdjacentPieces = 1;
        int colAdjacentPieces = 1;

        for (int i = 1; i < BOARD_SIZE - 1; i++)
        {
            for (int j = 1; j < BOARD_SIZE - 1; j++)
            {
                if (_board[i][j] == piece && _board[i][j] == _board[i][j + 1])
                    rowAdjacentPieces++;
                else
                    rowAdjacentPieces = 1;

                if (_board[j][i] == piece && _board[j][i] == _board[j + 1][i])
                    colAdjacentPieces++;
                else
                    colAdjacentPieces = 1;

                if (rowAdjacentPieces == connectionLength ||
                    colAdjacentPieces == connectionLength)
                    return true;
            }
        }

        return false;
    }

    int
    getFirstFreeRow(int column) const
    {
        if (column > 0 && column < BOARD_SIZE - 1)
            for (unsigned int i = 1; i < BOARD_SIZE - 1; i++)
                if (_board[i][column] == Piece::EMPTY)
                    return i;
        return -1;
    }
};

#endif