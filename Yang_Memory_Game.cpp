
/*
Name: Fong Yang
Status: Complete - 10/17/2026
Description: This program is a memory game with 16 cards (7-A)
             arranged in a 4x4 array. The player finds 8 matching
             pairs, earns a ranking, and can play again.
Created: 10/13/2026 FKY
Changed: 10/13/2026 - Psuedocode created FKY
Changed: 10/14/2026 - Code added in, improved coodrinate input validation, fixed card selection errors. FKY
Changed: 10/15/2026 - Changed card values because it was confusing while playing the game
                      to see the same numbers listed. FKY
Changed: 10/16/2026 - Added replay functionality. FKY
Changed: 10/16/2026 - Added scoring capabilities after replay. FKY
Program tested 10/17/2026
*/

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int ROWS = 4;
const int COLS = 4;

// Function prototypes
void shuffleCards(char cards[][COLS]);
void showCards(char cards[][COLS], bool matched[][COLS],
               int row1, int col1, int row2, int col2);
bool getCard(int &row, int &col);
int playGame();
void showRanking(int score, int high_scores[], int &score_count);
void startGames();

// Main function
int main() {
    startGames();
    return 0;
}

// This function puts the cards in random positions
void shuffleCards(char cards[][COLS]) {
    char numbers[16];
    char card_values[8] = {'A', 'K', 'Q', 'J', 'T', '9', '8', '7'};
    char temp;
    int random_position;
    int count = 0;

    // Make two of each card value
    for (int i = 0; i < 8; i++) {
        numbers[i * 2] = card_values[i];
        numbers[i * 2 + 1] = card_values[i];
    }

    // Mix up the cards
    for (int i = 0; i < 16; i++) {
        random_position = rand() % 16;

        temp = numbers[i];
        numbers[i] = numbers[random_position];
        numbers[random_position] = temp;
    }

    // Put the cards into the 4x4 array
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cards[i][j] = numbers[count];
            count++;
        }
    }
}

// This function shows the cards on the screen
void showCards(char cards[][COLS], bool matched[][COLS],
               int row1, int col1, int row2, int col2) {
    cout << endl;
    cout << "    1  2  3  4" << endl;

    for (int i = 0; i < ROWS; i++) {
        cout << i + 1 << "   ";

        for (int j = 0; j < COLS; j++) {
            if (matched[i][j] == true ||
                (i == row1 && j == col1) ||
                (i == row2 && j == col2)) {

                // Display T as 10
                if (cards[i][j] == 'T') {
                    cout << "10 ";
                }
                else {
                    cout << cards[i][j] << "  ";
                }
            }
            else {
                cout << "*  ";
            }
        }

        cout << endl;
    }
}

// This function checks the row and column input
bool getCard(int &row, int &col) {
    char input[100];
    bool valid = false;

    while (valid == false) {
        cin.getline(input, 100);

        if (!cin) {
            return false;
        }

        // Check for two numbers entered together
        if (input[0] >= '1' && input[0] <= '4') {
            if (input[1] >= '1' && input[1] <= '4' &&
                input[2] == '\0') {
                row = input[0] - '1';
                col = input[1] - '1';
                valid = true;
            }

            // Check for two numbers separated by a space
            else if (input[1] == ' ' &&
                     input[2] >= '1' && input[2] <= '4' &&
                     input[3] == '\0') {
                row = input[0] - '1';
                col = input[2] - '1';
                valid = true;
            }
        }

        // Ask again if the input is invalid
        if (valid == false) {
            cout << "Invalid input. Enter two numbers from 1 to 4 "
                 << "(such as 13 or 1 3): ";
        }
    }

    return true;
}

// This function runs one memory game
int playGame() {
    char cards[ROWS][COLS];
    bool matched[ROWS][COLS] = {false};

    int row1, col1;
    int row2, col2;
    int pairs = 0;
    int attempts = 0;
    bool valid_second;

    // Shuffle the cards for each new game
    shuffleCards(cards);

    cout << "\nMEMORY GAME" << endl;
    cout << "Find all 8 matching pairs." << endl;
    cout << "Enter both coordinates on one line, like 13 or 1 3." << endl;

    while (pairs < 8) {
        // Show the cards that have already been matched
        showCards(cards, matched, -1, -1, -1, -1);

        // Get the first card
        cout << endl;
        cout << "Enter the row and column of the first card (1-4): ";

        if (!getCard(row1, col1)) {
            return 0;
        }

        // Check the first card
        if (matched[row1][col1] == true) {
            cout << "You've already guessed that card. Try a different one." << endl;
            continue;
        }

        showCards(cards, matched, row1, col1, -1, -1);

        // Get the second card, keeping the first card visible
        valid_second = false;

        while (valid_second == false) {
            cout << endl;
            cout << "Enter the row and column of the second card (1-4): ";

            if (!getCard(row2, col2)) {
                return 0;
            }

            // Check the second card
            if (matched[row2][col2] == true ||
                (row1 == row2 && col1 == col2)) {
                cout << "You've already guessed that card. Try a different one." << endl;
                showCards(cards, matched, row1, col1, -1, -1);
            }
            else {
                valid_second = true;
            }
        }

        // Count one attempt for each valid pair selected
        attempts++;

        // Show both selected cards
        showCards(cards, matched, row1, col1, row2, col2);

        // See if the cards match
        if (cards[row1][col1] == cards[row2][col2]) {
            cout << "You found a match!" << endl;

            matched[row1][col1] = true;
            matched[row2][col2] = true;

            pairs++;
        }
        else {
            cout << "The cards do not match." << endl;
        }

        cout << "Pair attempts: " << attempts << endl;
    }

    cout << endl;
    cout << "You found all 8 pairs. You win!" << endl;

    return attempts;
}

// This function records the three best different scores
void showRanking(int score, int high_scores[], int &score_count) {
    int place = 0;
    bool tied = false;

    // Check whether this score is already on the board
    for (int i = 0; i < score_count; i++) {
        if (score == high_scores[i]) {
            place = i + 1;
            tied = true;
        }
    }

    // Find where a new score belongs
    if (tied == false) {
        for (int i = 0; i < score_count; i++) {
            if (score < high_scores[i] && place == 0) {
                place = i + 1;
            }
        }

        if (place == 0 && score_count < 3) {
            place = score_count + 1;
        }

        if (place > 0) {
            // Move lower scores down to make room
            for (int i = 2; i >= place; i--) {
                high_scores[i] = high_scores[i - 1];
            }

            high_scores[place - 1] = score;

            if (score_count < 3) {
                score_count++;
            }
        }
    }

    // Display the player's placement
    if (tied == true) {
        if (place == 1) {
            cout << "** You tied with first! **" << endl;
        }
        else if (place == 2) {
            cout << "** You tied with second! **" << endl;
        }
        else {
            cout << "** You tied with third! **" << endl;
        }
    }
    else if (place == 1) {
        cout << "** Congratulations, you placed 1st! **" << endl;
    }
    else if (place == 2) {
        cout << "** Great job, you placed 2nd! **" << endl;
    }
    else if (place == 3) {
        cout << "** Good job, you placed 3rd! **" << endl;
    }
    else {
        cout << "You did not place in the top 3. Try again!" << endl;
    }

    cout << "Attempts this game: " << score << endl;

    // Display the three best scores
    cout << endl;
    cout << "HIGH SCORES" << endl;

    for (int i = 0; i < score_count; i++) {
        if (i == 0) {
            cout << "1st Place: ";
        }
        else if (i == 1) {
            cout << "2nd Place: ";
        }
        else {
            cout << "3rd Place: ";
        }

        cout << high_scores[i] << " attempts" << endl;
    }
}

// This function lets the player start another game
void startGames() {
    int high_scores[3] = {0};
    int score_count = 0;
    int score;
    char input[100];
    char again = 'Y';
    bool valid;

    // Initialize the random-number generator once
    srand(time(0));

    while (again == 'Y' || again == 'y') {
        score = playGame();

        if (score == 0) {
            return;
        }

        showRanking(score, high_scores, score_count);

        // Ask whether the player wants another game
        valid = false;

        while (valid == false) {
            cout << endl;
            cout << "Play again? (Y/N): ";
            cin.getline(input, 100);

            if (!cin) {
                return;
            }

            if ((input[0] == 'Y' || input[0] == 'y' ||
                 input[0] == 'N' || input[0] == 'n') &&
                input[1] == '\0') {
                again = input[0];
                valid = true;
            }
            else {
                cout << "Invalid input. Please enter Y or N." << endl;
            }
        }
    }
}
