#include <iostream>
#include <cctype>
#include <string>
#include <utility>

using namespace std;

class Board {
private:
    struct Player {
        string name {};
        char id {};
        int wins = 0;
    };
    Player players[2];
    int rows = 0;
    int columns = 0;
    string cells;
public:
    // Mutator Functions
    void setName(int i, string name) { players[i].name = move(name); }
    void setID(int i, char ID) { players[i].id = ID; }
    void addWin(int i) { players[i].wins++; }
    void setRows(int r) { rows = r; }
    void setColumns(int c) { columns = c; }
    void setTiles(int i, char t) { cells[i] = t; }
    void addTiles(char t) { cells.push_back(t); }

    // Accessor Functions
    Player getPlayer(int i) { return players[i]; }
    char getCell(int i) { return cells[i]; }
    int getRows() const { return rows; }
    int getColumns() const { return columns; }
};

// Function Prototypes
void startBoard(Board& board);
void displayBoard(Board& board);
void playerDrop(Board& board, int activePlayer);
int checkFourWin(Board& board, int activePlayer);
int fullBoard(Board& board);
void playerWin(Board& board, int activePlayer);
bool restart(Board& board);

int main() {
    Board board;
    string playerChoice;
    int isDigit,
        full,
        win;
    bool again = true;

    // Welcome message, starting with the board size
    cout << "Welcome to Connect 4!\n"
            "1) Default board size (6x7)\n"
            "2) Custom board size\n";

    // Ensures player inputs a valid option that is a digit and within 1-2
    while (!isDigit || !(stoi(playerChoice) >= 1 && stoi(playerChoice) <= 2)) {
        cout << "Select your board option (1-2): ";
        cin >> playerChoice;
        isDigit = isdigit(playerChoice[0]);
    }

    // If 1 is selected, set default rows and columns to 6 and 7 respectively
    if (stoi(playerChoice) == 1) {
        board.setRows(6);
        board.setColumns(7);
    }

    // If 2 is selected, ask player for custom rows and columns value and sets them respectively
    if (stoi(playerChoice) == 2) {
        isDigit = false;
        // Keeps running if input is not a digit or more than 0, then sets the rows
        while (!isDigit || stoi(playerChoice) <= 0) {
            // Asks for custom row size and sets it after
            cout << "Please enter the amount of rows: ";
            cin >> playerChoice;
            isDigit = isdigit(playerChoice[0]);
        }
        board.setRows(stoi(playerChoice));

        isDigit = false;
        // Keeps running if input is not a digit or more than 0, then sets the columns
        while (!isDigit || stoi(playerChoice) <= 0) {
            // Asks for custom column size and sets it after
            cout << "Please enter the amount of columns: ";
            cin >> playerChoice;
            isDigit = isdigit(playerChoice[0]);
        }
        board.setColumns(stoi(playerChoice));
    }

    // Loops through the two players for custom characters
    for (int i = 0; i < 2; i++) {
        cout << "Player " << (i + 1) << " please enter your character: ";
        cin >> playerChoice;

        // Checks to ensure only 1 character is entered
        while (playerChoice.length() != 1) {
            cout << "Player " << (i + 1) << " please enter only one character: ";
            cin >> playerChoice;
        }

        // Checks when it is Player 2's turn
        if (i == 1) {
            // Checks to ensure only 1 character is entered
            while (playerChoice.length() != 1) {
                cout << "Player 2 please enter only one character: ";
                cin >> playerChoice;
            }
            
            // Checks to ensure they do not have the same character as Player 1
            while (playerChoice[0] == board.getPlayer(0).id) {
                cout << "Player 2 please enter a different character than Player 1: ";
                cin >> playerChoice;
            }
        }

        // Sets appropriate player name & id and clears the string
        board.setName(i, "Player " + to_string(i + 1));
        board.setID(i, playerChoice[0]);
        playerChoice.clear();
    }

    // Constructs the empty board game
    startBoard(board);

    // Repeats the functions that make the game work, repeating through Player 1 and Player 2
    do {
        for (int i = 0; i < 2; i++) {
            displayBoard(board);
            playerDrop(board, i);
            win = checkFourWin(board, i);
            if (win == 1) {
                displayBoard(board);
                playerWin(board, i);
                again = restart(board);
                if (!again) {
                    break;
                }
            }
        }

        // Checks if the board is full
        full = fullBoard(board);
        if (full == board.getColumns()) {
            cout << "The board is full, it is a draw!" << endl;
            again = restart(board);
        }
    } while (again);

    return 0;
}

// Initializes or resets the board with *'s depending on the amount of rows and columns
void startBoard(Board& board) {
    // Checks if the board is empty, then initialize board with *'s
    if (board.getCell(0) != board.getPlayer(0).id &&
        board.getCell(0) != board.getPlayer(1).id &&
        board.getCell(0) != '*')
    {
        // Loops through the specified dimensions of board to add tiles
        for (int i = 0; i < (board.getRows() * board.getColumns()); i++) {
            board.addTiles('*');
        }
    }

    // If the board is not empty, then resets the board with *'s
    if (board.getCell(0) == board.getPlayer(0).id ||
        board.getCell(0) == board.getPlayer(1).id ||
        board.getCell(0) == '*')
    {
        // Loops through the specified dimensions of board to reset all tiles
        for (int i = 0; i < (board.getRows() * board.getColumns()); i++) {
            board.setTiles(i, '*');
        }
    }
}

// Goes through the string of the board and outputs it to player
void displayBoard(Board& board) {
    // Loops through the amount of rows specified
    for (int i = 0; i < board.getRows(); i++) {
        cout << "|";

        // Loops through the amount of columns specified for the board's string data
        for (int j = 0; j < board.getColumns(); j++) {
            cout << board.getCell((i * board.getColumns()) + j);
        }

        cout << "|\n";
    }

    cout << endl;
}
// Drops the player's ID, and checks if the column is full
void playerDrop(Board& board, int activePlayer) {
    // Column that player has selected
    int dropChoice;

    // Indicate the player's turn
    cout << "Player " << (activePlayer + 1) << "'s Turn\n";

    // Keeps asking player to select option if they enter invalid option
    do {
        cout << "Select an option (1-" << board.getColumns() << "): ";
        cin >> dropChoice;

        // If the top of the row of the dropChoice column has an ID, then make player enter another option
        while (board.getCell(dropChoice - 1) == board.getPlayer(0).id ||
               board.getCell(dropChoice - 1) == board.getPlayer(1).id)
        {
            cout << "Column " << dropChoice << " is full, please enter a new option: ";
            cin >> dropChoice;
        }

    } while (dropChoice < 1 || dropChoice > board.getColumns());

    // Sets row to be able to run through board in next function
    int row = board.getRows() - 1;
    // Detects when activePlayer's turn has or hasn't ended
    bool endTurn = false;

    // Repeats through the rows as long as the player's turn has not ended
    do {
        /* If the column (dropChoice) where the player dropped does not contain another player, puts the activePlayer's ID
           and ends their turn */
        if (board.getCell((row * board.getColumns()) + (dropChoice - 1)) != board.getPlayer(0).id &&
            board.getCell((row * board.getColumns()) + (dropChoice - 1)) != board.getPlayer(1).id)
        {
            board.setTiles((row * board.getColumns()) + (dropChoice - 1), board.getPlayer(activePlayer).id);
            endTurn = true;
        }

        // Go to the next row (upwards in board) if current row has a player in it
        else {
            row--;
        }
    } while (!endTurn);
}

// Checks four (pun intended) the four instances where player can connect 4, and returns that they won
int checkFourWin(Board& board, int activePlayer) {
    char ID = board.getPlayer(activePlayer).id;
    int win = 0;

    for (int i = board.getRows(); i >= 1; i--) {
        for (int j = board.getColumns(); j >= 1; j--) {

            if (board.getCell((i * board.getColumns()) + j) == ID         && //    X
                board.getCell(((i-1) * board.getColumns()) + (j-1)) == ID && //   X
                board.getCell(((i-2) * board.getColumns()) + (j-2)) == ID && //  X
                board.getCell(((i-3) * board.getColumns()) + (j-3)) == ID)   // X
            {
                win = 1;
            }

            if (board.getCell((i * board.getColumns()) + j) == ID         && // X
                board.getCell(((i-1) * board.getColumns()) + j) == ID     && // X
                board.getCell(((i-2) * board.getColumns()) + j) == ID     && // X
                board.getCell(((i-3) * board.getColumns()) + j) == ID)       // X
            {
                win = 1;
            }

            if (board.getCell((i * board.getColumns()) + j) == ID         && // X
                board.getCell(((i-1) * board.getColumns()) + (j+1)) == ID && //  X
                board.getCell(((i-2) * board.getColumns()) + (j+2)) == ID && //   X
                board.getCell(((i-3) * board.getColumns()) + (j+3)) == ID)   //    X
            {
                win = 1;
            }

            if (board.getCell((i * board.getColumns()) + j) == ID         && // X X X X
                board.getCell((i * board.getColumns()) + (j+1)) == ID     && //
                board.getCell((i * board.getColumns()) + (j+2)) == ID     && //
                board.getCell((i * board.getColumns()) + (j+3)) == ID)       //
            {
                win = 1;
            }
        }
    }

    return win;
}

// Checks if the top row of the board is full
int fullBoard(Board& board) {
    int full = 0;

    // Loops through columns and checks if top row does not have an empty cell
    for (int i = 0; i < board.getColumns(); i++) {
        if (board.getCell(i) != '*')
            full++;
    }

    return full;
}

// Announces player win and adds 1 to their win count
void playerWin(Board& board, int activePlayer) {
    board.addWin(activePlayer);
    cout << board.getPlayer(activePlayer).name << " has won!\n";
    cout << "Current Wins\n"
            "Player 1: " << board.getPlayer(0).wins << endl <<
            "Player 2: " << board.getPlayer(1).wins << endl << endl;
}

// Once game is over, ask player if they would like to restart their game
bool restart(Board& board) {
    int restart,
        isDigit;
    bool result;
    string playerChoice;

    // Ensures player inputs a valid option that is a digit and within 1-2
    while (!isDigit || !(stoi(playerChoice) >= 1 && stoi(playerChoice) <= 2)) {
        // Asks player if they would like to restart
        cout << "Would you like to restart? \n"
            "1) Yes \n"
            "2) No \n"
            "Select an option (1-2): ";
        cin >> playerChoice;
        isDigit = isdigit(playerChoice[0]);
    }
    restart = stoi(playerChoice);

    // Option 1 will reset the board and return true
    if (restart == 1) {
        startBoard(board);
        result = true;
    }
    // Option 2 will output a goodbye message and return false
    else if (restart == 2) {
        cout << "Goodbye!";
        result = false;
    }
    // If player didn't choose a valid option, ask player to select an option again
    else {
        cout << "Select an option (1-2): ";
        cin >> restart;
    }

    return result;
}