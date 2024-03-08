#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>

#define MAX_SIDE 9
#define MIN_TOKENS 3

#define RED_SLOT_SYMBOL ('R')
#define YELLOW_SLOT_SYMBOL ('Y')
#define EMPTY_SLOT_SYMBOL (' ')
#define MEMORY_SIZE 100


//----------------------- Message Identifiers --------------------------//
#define MSG_GET_BOARD_ROWS   0
#define MSG_GET_BOARD_COLS   1
#define MSG_GET_NUMBER_TOKENS 2


//--------------------------- Board Edges ------------------------------//

#define BOARD_VERT_SEP  '|'
#define BOARD_LEFT_ANG '\\'
#define BOARD_RIGHT_ANG '/'
#define BOARD_BOTTOM  '-'
#define BOARD_BOTTOM_SEP '-'



//----------------------------------------------------------------------//


void print_welcome_message();
void print_read_game_params_message(int param);
void print_chose_color_message();
void print_chose_move_message(int player);
void print_enter_column_message();
void print_full_column_message();
void print_unavailable_undo_redo_message();


/*
 * Outputs game board.
 *
 * board - contains spaces for empty cells, 'Y' or 'R' for
 *         respective player cells.
 * board_side - contains board dimensions {rows, cols, tokens}
 */
void print_board(char board[][MAX_SIDE], int board_side[]);

/*
 * Outputs winner or tie message.
 *
 * player_id - the id to be output. Pass <0 for tie.
 */
void print_winner(int player_id);
// makes the board contain only spaces
void initialize_board(char board[][MAX_SIDE]);
/*
 * gets an input of the desired first color
 * and prints two starting sentences
 */
int get_first_piece_n_print(char pieces[2]);
// calculates the min between 2 numbers
int min(int num1, int num2);
/*
 * takes input for the desired board size and number of token to win
 */
int get_rows_cols_tokens(int board_side[], int* tokens);
/*
 * check the vadality of the dimensions and number of tokens inputed
 */
bool check_valid_parms(int board_side[], int i, int* tokens);
/*
 * passed the turn the the next player
 */
void next_player(int* p_player);
/*
 * inserts a piece inside the board if it is valid
 */
int insert_piece(char board[][MAX_SIDE], int board_side[],
    int memory[][4], char piece, int* p_counter, int* p_player, int col);
/*memory_arr - contains the last moves of the players by
               {row_played, col_played, player, memory used}
               memory used - to indicate if the row has been used before
*/

/*
 * puts togheter all the main functions of the game
 */
int game_core(char board[][MAX_SIDE], int board_side[],
    int memory[][4], char pieces[], int* p_player, int tokens);
/*
 * inserts data in memory array
 */
void insert_in_memory(int memory[][4], int counter, int* p_player,
    int last_row, int last_col);
/*
 * validates the input of desired column
 * 
 * acts upon the case in dependents to the column value
 */
int col_cases(char board[][MAX_SIDE], int board_side[], char pieces[],
    int* p_player, int memory[][4], int* p_counter, int* col);
/*
 * recives the column the player wishes to place a piece
 */
int get_wanted_column(int board_side[]);
//checks if theres a tie
bool check_tie(char board[][MAX_SIDE], int board_side[], bool game_won);
//checks if theres a win for a player
bool check_win(char board[][MAX_SIDE], int board_side[],
    int tokens, char symbol, int row, int col, int* p_player);
// checks the possible win case of the desired num of tokens in a row
bool check_row(char board[][MAX_SIDE], int board_side[],
    int tokens, char symbol, int row, int col);
int count_row_L(char board[][MAX_SIDE],
    int tokens, char symbol, int row, int col);
int count_row_R(char board[][MAX_SIDE], int board_side[],
    int tokens, char symbol, int row, int col);
// checks the possible win case of the desired num of tokens in a column
bool check_col_D(char board[][MAX_SIDE], int board_side[],
    int tokens, char symbol, int row, int col);
/*
 *checks the possible win case of the desired num of 
 *tokens in a diagonal
 */
bool check_dig_main(char board[][MAX_SIDE], int board_side[],
    int tokens, char symbol, int row, int col);
int count_dig_main_R(char board[][MAX_SIDE], int board_side[],
    int tokens, char symbol, int row, int col);
int count_dig_main_L(char board[][MAX_SIDE],
    int tokens, char symbol, int row, int col);
/*
 *checks the possible win case of the desired num of 
 *tokens in the other diagonal
 */
bool check_dig_sec(char board[][MAX_SIDE], int board_side[],
    int tokens, char symbol, int row, int col);
int count_dig_sec_R(char board[][MAX_SIDE], int board_side[],
    int tokens, char symbol, int row, int col);
int count_dig_sec_L(char board[][MAX_SIDE], int board_side[],
    int tokens, char symbol, int row, int col);
// undos a move
int undo(char board[][MAX_SIDE], int memory[][4], int* p_counter);
// redos a move thats been undoed 
int redo(char board[][MAX_SIDE], int memory[][4], int* p_counter,
    char pieces[]);
// cleans the memory array from a certin point
void clean_memory(int memory[][4], int counter);
//--------------------------- Main Program -----------------------------//

// length = 11
// scanf_check makes sure all inputs are valid
// might be better to have a function that does the checks for scanf
int main()
{
    char board[MAX_SIDE][MAX_SIDE], pieces[] = { 'Y', 'R' };
    int scanf_check = 0, board_side[] = { 0, 0, 0 }, tokens = MIN_TOKENS;
    int* p_tokens = &tokens, player = 1, * p_player = &player;
    int memory_arr[MEMORY_SIZE][4] = { {0, 0, 0, 0} };
    scanf_check = get_first_piece_n_print(pieces);
    scanf_check = get_rows_cols_tokens(board_side, p_tokens);
    initialize_board(board);
    print_board(board, board_side);
    scanf_check = game_core(board, board_side, memory_arr,
        pieces, p_player, tokens);
    return scanf_check;
}

// length = 13
int game_core(char board[][MAX_SIDE], int board_side[],
    int memory[][4], char pieces[], int* p_player, int tokens)
{
    bool game_won = false, game_tie = false;
    int col = 0, *p_col = &col, row, counter = 0, * p_counter = &counter;
    while (!game_won && !game_tie)
    {
        next_player(p_player);
        print_chose_move_message(*p_player + 1);
        row = col_cases(board, board_side, pieces, p_player, 
                        memory, p_counter, p_col);
        if (row == -1) {
            return 1;
        }
        game_won = check_win(board, board_side, tokens,
            pieces[*p_player], row, col, p_player);
        game_tie = check_tie(board, board_side, game_won);
    }
    return 0;
}

// length = 4
void insert_in_memory(int memory[][4], int counter, int* p_player,
    int last_row, int last_col)
{
    memory[counter][0] = last_row;
    memory[counter][1] = last_col;
    memory[counter][2] = *p_player;
    memory[counter][3] = 1;
}

// length = 2
void clean_memory(int memory[][4], int counter) 
{
    for (int i = counter; i < MEMORY_SIZE; i++) {
        memory[i][3] = 0;
    }
}

// length = 13
int col_cases(char board[][MAX_SIDE], int board_side[], char pieces[],
    int* p_player, int memory[][4], int* p_counter, int *p_col)
{
    int row_inserted = -1;
    while (row_inserted == -1)
    {
        *p_col = get_wanted_column(board_side);
        if (*p_col == 0) {
            return -1;
        }
        else if (*p_col == -1)
        {
            row_inserted = undo(board, memory, p_counter);
        }
        else if (*p_col == -2)
        {
            row_inserted = redo(board, memory, p_counter, pieces);
        }
        else
        {
            row_inserted = insert_piece(board, board_side, memory,
                pieces[*p_player], p_counter, p_player, --*p_col);
        }
    }
    print_board(board, board_side);
    return row_inserted;
}

// length = 7
int undo(char board[][MAX_SIDE], int memory[][4], int* p_counter)
{
    if (*p_counter == 0) {
        print_unavailable_undo_redo_message();
        return -1;
    }
    *p_counter -= 1;
    board[memory[*p_counter][0]][memory[*p_counter][1]] =
        EMPTY_SLOT_SYMBOL;
    return MAX_SIDE + 1;
}

// length = 8
int redo(char board[][MAX_SIDE], int memory[][4], int* p_counter,
    char pieces[])
{
    int mem_row = *p_counter;
    if (memory[*p_counter][3] == 0) {
        print_unavailable_undo_redo_message();
        return -1;
    }
    board[memory[mem_row][0]][memory[mem_row][1]] =
        pieces[memory[mem_row][2]];
    *p_counter += 1;
    return MAX_SIDE + 1;
}

// length = 13
int insert_piece(char board[][MAX_SIDE], int board_side[],
    int memory[][4], char piece, int* p_counter, int* p_player, int col)
{
    int curr_row = board_side[0] - 1;
    while (true)
    {
        if (board[curr_row][col] == ' ')
        {
            board[curr_row][col] = piece;
            insert_in_memory(memory, *p_counter, p_player, curr_row, col);
            *p_counter += 1;
            clean_memory(memory, *p_counter);
            return curr_row;
        }
        else if (curr_row == 0)
        {
            print_full_column_message();
            break;
        }
        --curr_row;
    }
    return -1;
}

// length = 9
bool check_win(char board[][MAX_SIDE], int board_side[],
    int tokens, char symbol, int row, int col, int* p_player)
{
    if (row == MAX_SIDE + 1)
    {
        return false;
    }
    if (check_row(board, board_side, tokens, symbol, row, col) ||
        check_col_D(board, board_side, tokens, symbol, row, col) ||
        check_dig_main(board, board_side, tokens, symbol, row, col) ||
        check_dig_sec(board, board_side, tokens, symbol, row, col)) {
        print_winner(*p_player + 1);
        return true;
    }
    return false;
}

// length = 9
bool check_tie(char board[][MAX_SIDE], int board_side[], bool game_won)
{
    int count = 0;
    if (!game_won)
    {
        for (int i = 0; i < board_side[1]; i++) {
            if (board[0][i] != EMPTY_SLOT_SYMBOL) {
                count++;
            }
            if (count == board_side[1]) {
                print_winner(-1);
                return true;
            }
        }
    }
    return false;
}

// length = 6
bool check_row(char board[][MAX_SIDE], int board_side[],
    int tokens, char symbol, int row, int col)
{
    int count = 1;
    count += count_row_R(board, board_side, tokens, symbol, row, col);
    count += count_row_L(board, tokens, symbol, row, col);
    if (count >= tokens)
    {
        return true;
    }
    return false;
}

// length = 9
int count_row_L(char board[][MAX_SIDE],
    int tokens, char symbol, int row, int col)
{
    int count = 0;
    for (int i = 1; i < tokens; i++) {
        if (col - i == -1) {
            break;
        }
        if (board[row][col - i] == symbol) {
            count++;
        }
        else {
            break;
        }
    }
    return count;
}

// length = 9
int count_row_R(char board[][MAX_SIDE], int board_side[],
    int tokens, char symbol, int row, int col)
{
    int count = 0;
    for (int i = 1; i < tokens; i++) {
        if (col + i == board_side[1]) {
            break;
        }
        if (board[row][col + i] == symbol) {
            count++;
        }
        else {
            break;
        }
    }
    return count;
}

// length = 9
bool check_col_D(char board[][MAX_SIDE], int board_side[],
    int tokens, char symbol, int row, int col)
{
    int count = 0;
    for (int i = 0; i < tokens; i++) {
        if (row + i == board_side[0]) {
            break;
        }
        if (board[row + i][col] == symbol) {
            count++;
        }
        if (count == tokens) {
            return true;
        }
    }
    return false;

}

// length = 8
bool check_dig_main(char board[][MAX_SIDE], int board_side[],
    int tokens, char symbol, int row, int col)
{
    int count = 1;
    count += count_dig_main_R(board, board_side, tokens,
        symbol, row, col);
    count += count_dig_main_L(board, tokens,
        symbol, row, col);
    if (count >= tokens)
    {
        return true;
    }
    return false;

}

// length = 8
int count_dig_main_R(char board[][MAX_SIDE], int board_side[],
    int tokens, char symbol, int row, int col)
{
    int count = 0;
    for (int i = 1; i < tokens; i++) {
        if (row + i == board_side[0] || col + i == board_side[1]) {
            break;
        }
        if (board[row + i][col + i] == symbol) {
            count++;
        }
        else {
            break;
        }
    }
    return count;
}

// length = 8
int count_dig_main_L(char board[][MAX_SIDE],
    int tokens, char symbol, int row, int col)
{
    int count = 0;
    for (int i = 1; i < tokens; i++) {
        if (row - i == -1 || col - i == -1) {
            break;
        }
        if (board[row - i][col - i] == symbol) {
            count++;
        }
        else {
            break;
        }
    }
    return count;
}



// length = 8
bool check_dig_sec(char board[][MAX_SIDE], int board_side[],
    int tokens, char symbol, int row, int col)
{
    int count = 1;
    count += count_dig_sec_R(board, board_side, tokens,
        symbol, row, col);
    count += count_dig_sec_L(board, board_side, tokens,
        symbol, row, col);
    if (count >= tokens)
    {
        return true;
    }
    return false;

}

// length = 8
int count_dig_sec_R(char board[][MAX_SIDE], int board_side[],
    int tokens, char symbol, int row, int col)
{
    int count = 0;
    for (int i = 1; i < tokens; i++) {
        if (row - i == -1 || col + i == board_side[1]) {
            break;
        }
        if (board[row - i][col + i] == symbol) {
            count++;
        }
        else {
            break;
        }
    }
    return count;
}

// length = 8
int count_dig_sec_L(char board[][MAX_SIDE], int board_side[],
    int tokens, char symbol, int row, int col)
{
    int count = 0;
    for (int i = 1; i < tokens; i++) {
        if (row + i == board_side[0] || col - i == -1) {
            break;
        }
        if (board[row + i][col - i] == symbol) {
            count++;
        }
        else {
            break;
        }
    }
    return count;
}

// length = 3
void next_player(int* pointer)
{
    int temp = *pointer;
    temp = (temp + 1) % 2;
    *pointer = temp;
}

// length = 3
void initialize_board(char board[][MAX_SIDE])
{
    for (int i = 0; i < MAX_SIDE; i++) {
        for (int j = 0; j < MAX_SIDE; j++) {
            board[i][j] = EMPTY_SLOT_SYMBOL;
        }
    }
}

// length = 10
int get_wanted_column(int board_side[])
{
    bool valid_col = false;
    int col_input;
    while (!valid_col) {
        print_enter_column_message();
        if (scanf(" %d", &col_input) != 1) {
            return 0;
        }
        if (col_input != 0 && col_input <= board_side[1]
            && col_input >= -2) {
            return col_input;
        }
    }
    return 0;
}

// length = 12
int get_first_piece_n_print(char pieces[2])
{
    bool check = false;
    print_welcome_message();
    while (!check) {
        print_chose_color_message();
        if (scanf(" %c", &pieces[0]) != 1) {
            return 1;
        }
        if (pieces[0] == 'Y') {
            check = true;
        }
        else if (pieces[0] == 'R') {
            check = true;
            pieces[1] = 'Y';
        }
    }
    return 0;
}

// length = 3
int min(int num1, int num2)
{
    if (num1 < num2) {
        return num1;
    }
    return num2;
}

// length = 11 
int get_rows_cols_tokens(int board_side[], int* tokens)
{
    bool check;
    for (int i = 0; i < 3; i++)
    {
        check = false;
        while (!check) {
            print_read_game_params_message(i);
            if (scanf(" %d", &board_side[i]) != 1) {
                return 1;
            }
            if (i == 2) {
                *tokens = board_side[2];
            }
            check = check_valid_parms(board_side, i, tokens);
        }
    }
    return 0;
}

// length = 10
bool check_valid_parms(int board_side[], int i, int* tokens)
{
    if (i == 0) {
        if (board_side[0] <= MAX_SIDE && board_side[0] >= 3) {
            return true;
        }
    }
    else if (i == 1) {
        if (board_side[1] <= MAX_SIDE && board_side[1] >= 3) {
            return true;
        }
    }
    else {
        int max_tokens = min(board_side[0], board_side[1]);
        if (*tokens >= 3 && *tokens <= max_tokens) {
            return true;
        }
    }
    return false;
}

//---------------------- Printing Functions ----------------------------//

// message number 0 = "Welcome to 4-in-a-row game! \n"
void print_welcome_message()
{
    printf("Welcome to 4-in-a-row game! \n");
}


// message number 1 = "Please enter number of rows:"
// message number 2 = "Please enter number of columns:"
// message number 3 = "Please enter number of tokens:"
void print_read_game_params_message(int param)
{
    char const* const possible_params[] = { "rows", "columns", "tokens" };

    printf("Please enter number of %s: ", possible_params[param]);
}

// message number 4 = "Please choose starting color (Y)ellow or (R)ed: "
void print_chose_color_message()
{
    printf("Please choose starting color (Y)ellow or (R)ed: ");
}

// message number 5 = "Your move, player <player>. "
void print_chose_move_message(int player)
{
    printf("Your move, player %d. ", player);
}

// message number 6
void print_enter_column_message()
{
    printf("Please enter column: ");
}

void print_full_column_message()
{
    printf("Column full. ");
}

void print_unavailable_undo_redo_message()
{
    printf("No moves to undo/redo. ");
}

void print_board(char board[][MAX_SIDE], int board_side[])
{
    for (int row = 0; row < board_side[0]; ++row)
    {
        printf("\n%c", BOARD_VERT_SEP);
        for (int col = 0; col < board_side[1]; ++col)
            printf("%c%c", board[row][col], BOARD_VERT_SEP);
    }
    printf("\n%c", BOARD_LEFT_ANG);
    for (int ii = 0; ii < board_side[1] - 1; ++ii)
        printf("%c%c", BOARD_BOTTOM, BOARD_BOTTOM_SEP);
    printf("%c%c\n", BOARD_BOTTOM, BOARD_RIGHT_ANG);
}

void print_winner(int player_id)
{
    if (player_id >= 0)
        printf("Player %d won! \n", player_id);
    else
        printf("That's a tie. \n");
}
