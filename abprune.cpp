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
    int minimax(int depth, bool is_maximizing, int alpha, int beta); //find the answer
    void best_move(); // calls minimax
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

    // Check main diagonal
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

    // Check anti-diagonal
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

int tictactoe::minimax(int depth, bool is_maximizing, int alpha, int beta) {
    char winner = check_win();
    if (winner == 'X') return -5; //Player is winning this is BAD
    if (winner == 'O') return 5; //I am winning which is the way it SHOULD BE
    if (!has_moves()) return 0;   // No more moves means a draw

    if (is_maximizing) {
        int best_score = numeric_limits<int>::min();
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                if (board[row][col] == ' ') {  // Check if cell is empty
                    board[row][col] = 'O';    // Make the move
                    best_score = max(best_score, minimax(depth + 1, false, alpha, beta));
                    board[row][col] = ' ';    // Undo the move
                    alpha = max(alpha, best_score);
                    if (beta <= alpha) {
                        break;  // Alpha Beta Pruning
                    }
                }
            }
        }
        return best_score;
    } else {
        int best_score = numeric_limits<int>::max();
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                if (board[row][col] == ' ') {  // Check if cell is empty
                    board[row][col] = 'X';    // Make the move
                    best_score = min(best_score, minimax(depth + 1, true, alpha, beta));
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
    int best_val = numeric_limits<int>::min();
    int best_row = -1;
    int best_col = -1;

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (board[row][col] == ' ') {
                board[row][col] = 'O';
                int move_val = minimax(0, false, numeric_limits<int>::min(), numeric_limits<int>::max());
                board[row][col] = ' ';
                if (move_val > best_val) {
                    best_row = row;
                    best_col = col;
                    best_val = move_val;
                }
            }
        }
    }
    if (best_row != -1 && best_col != -1) {
        board[best_row][best_col] = 'O'; // AI plays as 'O'
    }
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

int main() {
    int size;
    cout << "Enter the size of your square (normal tic tac toe will be 3): ";
    cin >> size;

    tictactoe game(size); //call game 

    game.blank_space(); // call blank board

    while (true) {
        game.print_board(); // Print Board

        // Your Move move (no recursion required!)
        game.player_move();
        game.print_board();
        if (game.check_win() == 'X') {
            cout << "You won I guess-Did you cheat" << endl;
            break;
        }
        if (!game.has_moves()) { //if theres nothing you can do but you didn't win
            cout << "Its a tie! Best of 3?" << endl;
            break;
        }

        // My Move 
        game.best_move(); //MINIMAX CALL
        // game.print_board();
        if (game.check_win() == 'O') {
            cout << "Erin Wins-buy her a beer!!" << endl;
            break;
        }
        if (!game.has_moves()) {
            cout << "Its a tie! Best of 3?" << endl;
            break;
        }
    }

    return 0;
}
