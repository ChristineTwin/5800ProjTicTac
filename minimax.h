
#include <iostream>
#include <vector>
#include <limits>
#include <climits>

using namespace std;

class tictactoe {
public:
    tictactoe(int n); // Constructor with size
    ~tictactoe(); // Destructor
    void blank_space();
    void print_board() const;
    char check_win() const;
    int evaluate() const;
    bool has_moves() const;
    int minimax(int branch, bool is_maximizing, int=3);
    void best_move(); // Uses minimax function to make my move
    void player_move(); // Your move

private:
    int size; // Board size
    vector<vector<char> > board; // MATTTRIXES 
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
            cout << ' ' << board[r][c] << ' '; //these spaces stoooop
            if (c < size - 1) cout << "|"; //oh my god these indicies 
        }
        cout << endl;
        if (r < size - 1) {
            for (int c = 0; c < size; ++c) {
                cout << "---";
                if (c < size - 1) cout << "+"; //this is my nightmare 
            }
            cout << endl;
        }
    }
}

char tictactoe::check_win() const {
    // is this just going to be a fuck ton of if statements
    for (int row = 0; row < size; ++row) {
        if (board[row][0] != ' ' && equal(board[row].begin() + 1, board[row].end(), board[row].begin())) {
            return board[row][0];
        }
        bool col_win = true;
        for (int col = 1; col < size; ++col) {
            if (board[0][row] == ' ' || board[col][row] != board[0][row]) {
                col_win = false;
                break;
            }
        }
        if (col_win) {
            return board[0][row];
        }
    }

    // yep
    bool diag1_win = true, diag2_win = true;
    for (int row = 1; row < size; ++row) {
        if (board[0][0] == ' ' || board[row][row] != board[0][0]) {
            diag1_win = false;
        }
        if (board[0][size-1] == ' ' || board[row][size-1-row] != board[0][size-1]) {
            diag2_win = false;
        }
    }
    if (diag1_win) return board[0][0];
    if (diag2_win) return board[0][size-1];

    return ' '; 
}

bool tictactoe::has_moves() const {
    //CAN WE EVEN STILL PLAY
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (board[row][col] == ' ') {
                return true;
            }
        }
    }
    return false;
}

int tictactoe::evaluate() const {
    //We need this for bigger boards
    int score = 0;
    for (int row = 0; row < size; row++){
        for(int col = 0; col < size; col++){
            // Lets give squares in the middle more points
            // Since further from the edge means more ways to build off of
            int value = size - abs(row-int(size/2)) - abs(col-int(size/2));
            if(board[row][col] == 'O'){
                score = score + value;
            }
        }
    }
    return score;
}

int tictactoe::minimax(int branch, bool is_maximizing, int depth){                              // if is maximizing is false that is bad
    char winner = check_win(); //need to see if this is working so check it
    if (winner == 'X') return -size*size; //Player is winning this is BAD
    if (winner == 'O') return size*size; //I am winning which is the way it SHOULD BE
    if (!has_moves()) return 0; //These numbers are arbitrary 
    if (depth == 0){
        return evaluate();
    }
    if (is_maximizing) { //you need to maximize or you'll lose 
        int best = INT_MIN; // starts best as really low 
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                if (board[row][col] == ' ') {
                    board[row][col] = 'O'; // 
                    best = max(best, minimax(branch + 1, false, depth-1)); //RECURSION 
                    //This is simulating you playing and you are not maximizing 
                    board[row][col] = ' ';
                }
            }
        }
        return best;
    } else {
        int best = INT_MAX; //sets this as really high to minimize
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                if (board[row][col] == ' ') {
                    board[row][col] = 'X'; //Player
                    best = min(best, minimax(branch + 1, true, depth -1)); //I maximize
                    board[row][col] = ' ';
                }
            }
        }
        return best;
    }
}

void tictactoe::best_move() {
    int best_value = INT_MIN; //minimize to try and find the best move 
    int best_row = -1;
    int best_col = -1;

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (board[row][col] == ' ') {
                board[row][col] = 'O';
                int move_value = minimax(0, false, 3); 
                board[row][col] = ' ';
                if (move_value > best_value) {
                    best_row = row;
                    best_col = col;
                    best_value = move_value;
                }
            }
        }
    }
    board[best_row][best_col] = 'O'; //Erin Plays as O 
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
            cout << "This spots already taken! Try again" << endl;
        }
    }
}
