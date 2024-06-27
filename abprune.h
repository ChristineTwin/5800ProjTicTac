#include <iostream>
#include <vector>
#include <limits>

using namespace std;

class tictactoe {
public:
    tictactoe(int n); // Constructor with size
    ~tictactoe(); // Destructor
    void blank_space();
    void print_board() const;
    char check_win() const;
    bool has_moves() const;
    int evaluate() const;
    int minimax(int depth, bool is_maximizing, int alpha, int beta);
    void best_move(); // Uses minimax function to make AI move
    void player_move(); // Player move

private:
    int size; // Board size
    vector<vector<char> > board; // Matrix
};

tictactoe::tictactoe(int n) : size(n), board(n, vector<char>(n, ' ')) {}

// Destructor
tictactoe::~tictactoe() {}

void tictactoe::blank_space() {
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            board[r][c] = ' ';
        }
    }
}

void tictactoe::print_board() const {
    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            cout << ' ' << board[r][c] << ' ';
            if (c < size - 1) cout << "|";
        }
        cout << endl;
        if (r < size - 1) {
            for (int c = 0; c < size; ++c) {
                cout << "---";
                if (c < size - 1) cout << "+";
            }
            cout << endl;
        }
    }
}

char tictactoe::check_win() const {
    // Check rows
    for (int r = 0; r < size; ++r) {
        char first = board[r][0];
        if (first != ' ') {
            bool row_win = true;
            for (int c = 1; c < size; ++c) {
                if (board[r][c] != first) {
                    row_win = false;
                    break;
                }
            }
            if (row_win) {
                return first;
            }
        }
    }

    // Check columns
    for (int c = 0; c < size; ++c) {
        char first = board[0][c];
        if (first != ' ') {
            bool col_win = true;
            for (int r = 1; r < size; ++r) {
                if (board[r][c] != first) {
                    col_win = false;
                    break;
                }
            }
            if (col_win) {
                return first;
            }
        }
    }

    // Check diagonals
    char first = board[0][0];
    if (first != ' ') {
        bool diag_win = true;
        for (int i = 1; i < size; ++i) {
            if (board[i][i] != first) {
                diag_win = false;
                break;
            }
        }
        if (diag_win) {
            return first;
        }
    }

    first = board[0][size - 1];
    if (first != ' ') {
        bool diag_win = true;
        for (int i = 1; i < size; ++i) {
            if (board[i][size - 1 - i] != first) {
                diag_win = false;
                break;
            }
        }
        if (diag_win) {
            return first;
        }
    }

    return ' ';
}

bool tictactoe::has_moves() const {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (board[row][col] == ' ') {
                return true;
            }
        }
    }
    return false;
}

int tictactoe::evaluate() const{
    // We need this for bigger boards
    int score = 0;
    for (int row = 0; row < size; row++){
        for (int col = 0; col < size; col++){
            // Lets give squares in the middle more points
            // Since further from the edge means more ways to build off of
            int value = size - abs(row - int(size / 2)) - abs(col - int(size / 2));
            if (board[row][col] == 'O'){
                score = score + value;
            }
        }
    }
    return score;
}

int tictactoe::minimax(int depth, bool is_maximizing, int alpha, int beta) {
    char winner = check_win();
    if (winner == 'X') return -size*size; // Player 'X' winning is BAD for 'O'
    if (winner == 'O') return size*size;  // Player 'O' winning is GOOD
    if (!has_moves()) return 0;   // No more moves means a draw
    if (depth == 0){
        return evaluate();
    }

    if (is_maximizing) {
        int best_score = INT_MIN;
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                if (board[row][col] == ' ') {  // Check if cell is empty
                    board[row][col] = 'O';    // Make the move
                    best_score = max(best_score, minimax(depth - 1, false, alpha, beta));
                    board[row][col] = ' ';    // Undo the move
                    alpha = max(alpha, best_score);
                    if (beta <= alpha) {
                        break;  //
                    }
                }
            }
        }
        return best_score;
    } else {
        int best_score = INT_MAX;
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                if (board[row][col] == ' ') {  // Check if cell is empty
                    board[row][col] = 'X';    // Make the move
                    best_score = min(best_score, minimax(depth - 1, true, alpha, beta));
                    board[row][col] = ' ';    // Undo the move
                    beta = min(beta, best_score);
                    if (beta <= alpha) {
                        break;  // Alpha Beta Pruning
                    }
                }
            }
        }
        return best_score;
    }
}

void tictactoe::best_move() {
    int best_val = -100;
    int alpha = -100;
    int beta = 100;
    int best_row = -1;
    int best_col = -1;

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (board[row][col] == ' ') {
                board[row][col] = 'O';
                int move_val = minimax(3, false, alpha, beta);
                board[row][col] = ' ';
                if (move_val > best_val) {
                    best_row = row;
                    best_col = col;
                    best_val = move_val;
                }
            }
        }
    }
    board[best_row][best_col] = 'O'; // Play me where its BEST 

}

void tictactoe::player_move() {
    int row, col;
    while (true) {
        cout << "Enter your move (row and column): ";
        cin >> row >> col;
        if (row >= 0 && row < size && col >= 0 && col < size && board[row][col] == ' ') {
            board[row][col] = 'X';
            break;
        } else {
            cout << "This spot's already taken! Try again." << endl;
        }
    }
}