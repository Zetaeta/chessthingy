
#include <iostream>
#include <array>
#include <vector>

using namespace std;

#define WHITE 1
#define BLACK -1
#define BLANK 0

std::array<std::array<int, 5>, 5> board;

struct Coord {
    int x, y;
    int & operator*() {
        return board[x][y];
    }
};

//int & operator[](array<array<int, 5>, 5> &arr, Coord c) {
//    return arr[c.x][c.y];
//}


struct Move {
    Coord from, to;

    void apply() {
        *to = *from;
        *from = BLANK;
    }

    void undo() {
        *from = *to;
        *to = BLANK;
    }
};


Coord blank;


void setup() {
    for (int y=0; y<5; ++y) {
        for (int x=0; x<5; ++x) {
            if (x < y) {
                board[x][y] = BLACK;
            }
            else if (x == y && x >= 2) {
                if (x == 2) {
                    board[x][y] = BLANK;
                }
                else {
                    board[x][y] = BLACK;
                }
            }
            else {
                board[x][y] = WHITE;
            }
        }
    }
}

void print() {
    cout << "  a b c d e\n";
    for (int y=0; y<5; ++y) {
        cout << y << " ";
        for (int x=0; x<5; ++x) {
            int i = board[x][y];
            cout << (i ? (i == WHITE ? 'w' : 'b') : ' ') << ' ';
        }
        cout << '\n';
    }
}

bool canMove(Coord c) {
    int distx = abs(c.x - blank.x);
    int disty = abs(c.y - blank.y);
    return (distx == 2 && disty == 1) || (distx == 1 && disty == 2);
}

vector<Move> getPossibleMoves() {
    vector<Move> moves;
    for (int x=0; x<5; ++x) {
        for (int y=0; y<5; ++y) {
            if (canMove({x, y})) {
                moves.push_back({Coord{x, y}, blank});
            }
        }
    }
    return moves;
}

bool solved() {
    for (int y=0; y<5; ++y) {
        for (int x=0; x<5; ++x) {
            if (x < y) {
                if (board[x][y] != WHITE) {
                    return false;
                }
            }
            else if (x == y && x >= 2) {
                if (x != 2 && board[x][y] != WHITE) {
                    return false;
                }
            }
            else if (board[x][y] != WHITE) {
                return false;
            }
        }
    }
    return true;
}

int minCount;
vector<Move> minMoves;

void solve(vector<Move> &moves) {
    for (int i=0; i<moves.size(); ++i) cout << " ";
    cout << moves.size() << '\n';
    if (moves.size() > 25) {
        return;
    }
    vector<Move> possible = getPossibleMoves();
    for (Move move : possible) {
        move.apply();
        moves.push_back(move);
        if (solved()) {
            if (minCount == -1 || moves.size() < minCount) {
                minCount = moves.size();
                minMoves = moves;
            }
//            return true;
        }
        else {
            solve(moves);
        }
        moves[moves.size() - 1].undo();
        moves.erase(moves.end() - 1);
    }
}

int main() {
    setup();
    print();
    vector<Move> moves;
    cout << "About to solve\n";
    solve(moves);
    print();
    cout << "minCount = " << minCount;
}
