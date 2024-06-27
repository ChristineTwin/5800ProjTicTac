#include<iostream>
#include"minimax.h"

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
            cout << "Its a tie! Best of 3?"<< endl;
            break;
        }
    }

    return 0;
}
