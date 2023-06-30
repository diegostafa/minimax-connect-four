#ifndef CONNECT_FOUR_CPP
#define CONNECT_FOUR_CPP

#include <vector>
#include <limits>

class ConnectFour
{
public:
    static const int ROWS = 7;
    static const int COLS = 7;
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

    inline Piece
    pieceToPlay() const
    {
        return _turn ? Piece::PLAYER1 : Piece::PLAYER2;
    }

    inline bool
    isMaxTurn() const
    {
        return _turn;
    }

    inline bool
    isGameOver() const
    {
        return _isGameOver;
    }

    inline Piece
    pieceAt(int x, int y) const
    {
        return _board[x][y];
    }

    void
    makeMove(Move column)
    {
        if (_isGameOver)
            return;

        const int freeRow = getFirstFreeRow(column);

        if (freeRow != -1)
        {
            const Piece piece = pieceToPlay();
            _board[freeRow][column] = piece;
            _isGameOver = checkConnectFour(piece) || isBoardFull();

            if (!_isGameOver)
                _turn = !_turn;
        }
    }

    std::vector<Move>
    getPossibleMoves() const
    {
        std::vector<Move> possibleColumns;
        possibleColumns.reserve(COLS);

        for (int i = 0; i < COLS; i++)
            if (getFirstFreeRow(i) != -1)
                possibleColumns.push_back(i);

        return possibleColumns;
    }

    inline bool
    checkConnectFour(Piece piece) const
    {
        return checkConnectedPieces(piece, _connectionLength);
    }

    std::vector<std::pair<int, int>>
    getWinConnection() const
    {
        std::vector<std::pair<int, int>> winConnection;
        const Piece piece = pieceToPlay();
        winConnection.reserve(_connectionLength);

        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
            {
                if (j + _connectionLength <= COLS)
                {
                    bool connection = true;
                    for (int c = 0; c < _connectionLength; c++)
                    {
                        connection = connection && _board[i][j + c] == piece;
                        if (connection)
                            winConnection.push_back({i, j + c});
                    }
                    if (connection)
                        return winConnection;
                    else
                        winConnection.clear();
                }

                if (i + _connectionLength <= ROWS)
                {
                    bool connection = true;
                    for (int c = 0; c < _connectionLength; c++)
                    {
                        connection = connection && _board[i + c][j] == piece;
                        if (connection)
                            winConnection.push_back({i + c, j});
                    }
                    if (connection)
                        return winConnection;
                    else
                        winConnection.clear();
                }

                if (i + _connectionLength <= COLS && j + _connectionLength <= ROWS)
                {
                    bool connection = true;
                    for (int c = 0; c < _connectionLength; c++)
                    {
                        connection = connection && _board[i + c][j + c] == piece;
                        if (connection)
                            winConnection.push_back({i + c, j + c});
                    }
                    if (connection)
                        return winConnection;
                    else
                        winConnection.clear();
                }

                if (i - _connectionLength >= 0 && j + _connectionLength <= COLS)
                {
                    bool connection = true;
                    for (int c = 0; c < _connectionLength; c++)
                    {
                        connection = connection && _board[i - c][j + c] == piece;
                        if (connection)
                            winConnection.push_back({i - c, j + c});
                    }
                    if (connection)
                        return winConnection;
                    else
                        winConnection.clear();
                }
            }

        return winConnection;
    }

    bool
    isBoardFull() const
    {
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
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

        for (int i = 1; i < _connectionLength; i++)
        {
            valuePlayer1 +=
                checkConnectedPieces(Piece::PLAYER1, i) * 10 * i;

            valuePlayer2 +=
                checkConnectedPieces(Piece::PLAYER2, i) * 10 * i;
        }

        return valuePlayer1 - valuePlayer2;
    }

    void
    reset()
    {
        _isGameOver = false;
        _turn = true;

        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                _board[i][j] = Piece::EMPTY;
    }

private:
    Piece _board[ROWS][COLS];
    bool _turn;
    bool _isGameOver;
    const int _connectionLength = 4;

    bool
    checkConnectedPieces(Piece piece, int connectionLength) const
    {
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
            {
                if (j + connectionLength <= COLS)
                {
                    bool connection = true;
                    for (int c = 0; c < connectionLength; c++)
                        connection = connection && _board[i][j + c] == piece;

                    if (connection)
                        return connection;
                }

                if (i + connectionLength <= ROWS)
                {
                    bool connection = true;
                    for (int c = 0; c < connectionLength; c++)
                        connection = connection && _board[i + c][j] == piece;

                    if (connection)
                        return connection;
                }

                if (i + connectionLength <= COLS && j + connectionLength <= ROWS)
                {
                    bool connection = true;
                    for (int c = 0; c < connectionLength; c++)
                        connection = connection && _board[i + c][j + c] == piece;

                    if (connection)
                        return connection;
                }

                if (i - connectionLength >= 0 && j + connectionLength <= COLS)
                {
                    bool connection = true;
                    for (int c = 0; c < connectionLength; c++)
                        connection = connection && _board[i - c][j + c] == piece;

                    if (connection)
                        return connection;
                }
            }

        return false;
    }

    int
    getFirstFreeRow(Move column) const
    {
        if (column < COLS)
            for (int i = ROWS - 1; i >= 0; i--)
                if (_board[i][column] == Piece::EMPTY)
                    return i;
        return -1;
    }
};

#endif