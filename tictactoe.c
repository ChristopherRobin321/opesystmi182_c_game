#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

time_t t;
int row, col, x, y, px, py, slen, sx;


const int Xs = 1;
const int Os = 2;
const int BG = 3;


const int line_len = 7;                       

char break_lines[] = "-------";               
char play_lines[] = "| | | |";


char space_1 = ' ';
char space_2 = ' ';
char space_3 = ' ';
char space_4 = ' ';
char space_5 = ' ';
char space_6 = ' ';
char space_7 = ' ';
char space_8 = ' ';
char space_9 = ' ';
int space_1y, space_1x;
int space_2y, space_2x;
int space_3y, space_3x;
int space_4y, space_4x;
int space_5y, space_5x;
int space_6y, space_6x;
int space_7y, space_7x;
int space_8y, space_8x;
int space_9y, space_9x;

char *space_ptr = NULL;                       

// Player's side ('X' or 'O')
char side;                    

int running = 1;                              
int playing = 1;                              
int turn = 1;                                 
int game_over = 0;

// Function Declarations
void f_intro();                               
char f_setup();                               
void f_paint();                               
void f_turn_input();                          
void f_player_turn();                         
void f_AI_turn();                              
void f_evaluate_turn();                       
int f_AI_picker();                            
void f_declare_winner(char symbol);           


// Main Function
int main(){
    srand((unsigned) time(&t));
    initscr();
    clear();
    cbreak();
    keypad(stdscr, 1);
    curs_set(0);
    noecho();
    start_color();
    init_pair(Xs, COLOR_CYAN, COLOR_BLACK);
    init_pair(Os, COLOR_RED, COLOR_BLACK);
    init_pair(BG, COLOR_YELLOW, COLOR_BLACK);

    f_intro();                      
    getch();

    while(running){
        clear();
        side = f_setup();             
        playing = 1;
        while(playing){
            f_paint();                  
            f_turn_input();             
            turn++;
        }
        // To-Do, a reset function
    }

    endwin();
    return 0;
}

// Function Definitions
void f_intro(){                   
    // Print elaborate "animated" intro splash
    int which;
    clear();
    getmaxyx(stdscr, row, col);
    // Print the background
    for(y=0;y<=row;y++){
        for(x=0;x<=col;x++){
            which = rand() % 3;
            if(which == 0){
                // Print an "X" in the cell
                attron(COLOR_PAIR(Xs));
                mvprintw(y, x, "X");
                attroff(COLOR_PAIR(Xs));
            }else if(which == 1){
                // Print an "O" in the cell
                attron(COLOR_PAIR(Os));
                mvprintw(y, x, "O");
                attroff(COLOR_PAIR(Os));
            }else if(which == 2){
                // Print a blank black space in the cell
                attron(COLOR_PAIR(BG));
                mvprintw(y, x, " ");
                attroff(COLOR_PAIR(BG));
            }
        }
    }
    // Print the Title
    y = row / 2 - 1;
    char intro_str[] = " Tic Tac Toe! ";
    char intro_str_padding[] = "                      "; 
    char intro_str2[] = " Press any key to continue  "; 
    slen = strlen(intro_str);
    x = col / 2 - slen / 2;
    mvprintw(y++, x, intro_str_padding);
    mvprintw(y++, x, intro_str);
    mvprintw(y++, x, intro_str2);
    mvprintw(y, x, intro_str_padding);

    refresh();
}

char f_setup(){                   
    
    int input;
    clear();
    getmaxyx(stdscr, row, col);
    char setup_str1[] = "Pick a side!";
    char setup_str2[] = "Press 'X', 'O', or 'R' for Random!";
    char *chose_x = "You chose X's! Any key to continue...";
    char *chose_y = "You chose O's! Any key to continue...";
    char *choice_ptr = NULL;
    y = row / 2 - 1;
    slen = strlen(setup_str1);
    x = col / 2 - slen / 2;
    mvprintw(y++, x, setup_str1);
    slen = strlen(setup_str2);
    x = col / 2 - slen / 2;
    mvprintw(y++, x, setup_str2);
    y++;
    refresh();
    input = getch();
    if(input == 'X' || input == 'x'){
        choice_ptr = chose_x;
        slen = strlen(choice_ptr);
        x = col / 2 - slen / 2;
        mvprintw(y, x, choice_ptr);
        refresh();
        getch();
        return 'X';
    }else if(input == 'O' || input == 'o'){
        choice_ptr = chose_y;
        slen = strlen(choice_ptr);
        x = col / 2 - slen / 2;
        mvprintw(y, x, choice_ptr);
        refresh();
        getch();
        return 'O';
    }else if(input == 'R' || input == 'r'){
        int r;
        r = rand() % 2;
        if(r == 0){
            // Pick 'X'
            choice_ptr = chose_x;
            slen = strlen(choice_ptr);
            x = col / 2 - slen / 2;
            mvprintw(y, x, choice_ptr);
            refresh();
            getch();
            return 'X';
        }else if(r == 1){
            // Pick 'O'
            choice_ptr = chose_y;
            slen = strlen(choice_ptr);
            x = col / 2 - slen / 2;
            mvprintw(y, x, choice_ptr);
            refresh();
            getch();
            return 'O';
        }
    }else{
        char err_str[] = "Input error! Any key to continue...";
        slen = strlen(err_str);
        x = col / 2 - slen / 2;
        mvprintw(y, x, err_str);
        refresh();
        getch();
        f_setup();
    }
}

void f_paint(){                   
    // Paint the board state on a given turn
    /*
        1. Clear screen.
        2. Paint blank board.
        3. Paint the contents of each playable cell.
        4. Refresh screen
    */
    clear();                                                    
    getmaxyx(stdscr, row, col);                                 
    y = row / 2 - 3;                                            
    x = col / 2 - 3;                                            
    // Determine the locations of the 9 "playable" cells:
    space_1y = y + 1; space_1x = x + 1;
    space_2y = y + 1; space_2x = x + 3;
    space_3y = y + 1; space_3x = x + 5;
    space_4y = y + 3; space_4x = x + 1;
    space_5y = y + 3; space_5x = x + 3;
    space_6y = y + 3; space_6x = x + 5;
    space_7y = y + 5; space_7x = x + 1;
    space_8y = y + 5; space_8x = x + 3;
    space_9y = y + 5; space_9x = x + 5;
    // Paint the board roughly centered:
    int yy, xx;
    attron(COLOR_PAIR(BG));
    for(yy = 0; yy < line_len; yy++){
        if(yy == 0 || yy % 2 == 0){
            mvprintw(y + yy, x, break_lines);
        }else{
            mvprintw(y + yy, x, play_lines);
        }
    }
    attroff(COLOR_PAIR(BG));
    
    if(space_1 == 'X'){
        attron(COLOR_PAIR(Xs));
    }else if(space_1 == 'O'){
        attron(COLOR_PAIR(Os));
    }
    mvaddch(space_1y, space_1x, space_1);
    if(space_2 == 'X'){
        attron(COLOR_PAIR(Xs));
    }else if(space_2 == 'O'){
        attron(COLOR_PAIR(Os));
    }
    mvaddch(space_2y, space_2x, space_2);
    if(space_3 == 'X'){
        attron(COLOR_PAIR(Xs));
    }else if(space_3 == 'O'){
        attron(COLOR_PAIR(Os));
    }
    mvaddch(space_3y, space_3x, space_3);
    if(space_4 == 'X'){
        attron(COLOR_PAIR(Xs));
    }else if(space_4 == 'O'){
        attron(COLOR_PAIR(Os));
    }
    mvaddch(space_4y, space_4x, space_4);
    if(space_5 == 'X'){
        attron(COLOR_PAIR(Xs));
    }else if(space_5 == 'O'){
        attron(COLOR_PAIR(Os));
    }
    mvaddch(space_5y, space_5x, space_5);
    if(space_6 == 'X'){
        attron(COLOR_PAIR(Xs));
    }else if(space_6 == 'O'){
        attron(COLOR_PAIR(Os));
    }
    mvaddch(space_6y, space_6x, space_6);
    if(space_7 == 'X'){
        attron(COLOR_PAIR(Xs));
    }else if(space_7 == 'O'){
        attron(COLOR_PAIR(Os));
    }
    mvaddch(space_7y, space_7x, space_7);
    if(space_8 == 'X'){
        attron(COLOR_PAIR(Xs));
    }else if(space_8 == 'O'){
        attron(COLOR_PAIR(Os));
    }
    mvaddch(space_8y, space_8x, space_8);
    if(space_9 == 'X'){
        attron(COLOR_PAIR(Xs));
    }else if(space_9 == 'O'){
        attron(COLOR_PAIR(Os));
    }
    mvaddch(space_9y, space_9x, space_9);
    attroff(COLOR_PAIR(Xs));
    attroff(COLOR_PAIR(Os));
    refresh();
}

void f_turn_input(){              
    
    if(side == 'X'){
        // if player is 'X':
        f_player_turn();
        f_evaluate_turn();
        if(game_over == 0){
            f_AI_turn();
            f_evaluate_turn();
        }
    }else if(side == 'O'){
        // If player is 'O':
        f_AI_turn();
        f_evaluate_turn();
        if(game_over == 0){
            f_player_turn();
            f_evaluate_turn();
        }
    }
    refresh();
}


void f_player_turn(){             
    
    int info_line = y + 10;                               
    char move_splash[] = "Use arrow keys and press 'P' to place your piece!";
    char done_splash[] = "Good move!";
    char move_err_splash[] = "You can't move that way!";
    char input_err_splash[] = "Invalid input!";
    char full_err_splash[] = "Spot already claimed!";
    slen = strlen(move_splash);
    sx = col / 2 - slen / 2;                               
    mvprintw(info_line, sx, move_splash);
    curs_set(1);                                          
    int pos_y = space_1y;                                  
    int pos_x = space_1x;                                  
    move(pos_y, pos_x);                                    
    refresh();
    int inputting = 1;
    while(inputting){
        int input;
        char spot;
        int cx;
        input = getch();
        if(input == KEY_LEFT){
            if(!(pos_x == space_1x)){                          
                // If not on the left playable edge
                pos_x -= 2;
                move(pos_y, pos_x);
            }else{
                for(cx = sx; cx <= col; cx++){
                    // Clear the info line
                    mvaddch(info_line, cx, ' ');
                }
                slen = strlen(move_err_splash);
                sx = col / 2 - slen / 2;
                mvprintw(info_line, sx, move_err_splash);
                move(pos_y, pos_x);
            }
        }else if(input == KEY_RIGHT){
            if(!(pos_x == space_3x)){                          
                // If not on the right playable edge
                pos_x += 2;
                move(pos_y, pos_x);
            }else{
                for(cx = sx; cx <= col; cx++){
                    // Clear the info line
                    mvaddch(info_line, cx, ' ');
                }
                slen = strlen(move_err_splash);
                sx = col / 2 - slen / 2;
                mvprintw(info_line, sx, move_err_splash);   
                move(pos_y, pos_x);
            }
        }else if(input == KEY_UP){
            if(!(pos_y == space_1y)){                          
                // If not on the top playable edge
                pos_y -= 2;
                move(pos_y, pos_x);
            }else{
                for(cx = sx; cx <= col; cx++){
                    // Clear the info line
                    mvaddch(info_line, cx, ' ');
                }
                slen = strlen(move_err_splash);
                sx = col / 2 - slen / 2;
                mvprintw(info_line, sx, move_err_splash);   
                move(pos_y, pos_x);                                           
            }
        }else if(input == KEY_DOWN){
            if(!(pos_y == space_9y)){                          
                // If not on the bottom playable edge
                pos_y += 2;
                move(pos_y, pos_x);
            }else{
                for(cx = sx; cx <= col; cx++){
                    // Clear the info line
                    mvaddch(info_line, cx, ' ');
                }
                slen = strlen(move_err_splash);
                sx = col / 2 - slen / 2;
                mvprintw(info_line, sx, move_err_splash);   
                move(pos_y, pos_x);
            }
        }else if(input == 'P' || input == 'p'){
            /*
                1. Read contents of space.
                2. If Empty -> Place player's symbol
                3. Else, try again
            */
            if(pos_y == space_1y && pos_x == space_1x){
                space_ptr = &space_1;   
            }else if(pos_y == space_2y && pos_x == space_2x){
                space_ptr = &space_2;
            }else if(pos_y == space_3y && pos_x == space_3x){
                space_ptr = &space_3;
            }else if(pos_y == space_4y && pos_x == space_4x){
                space_ptr = &space_4;
            }else if(pos_y == space_5y && pos_x == space_5x){
                space_ptr = &space_5;
            }else if(pos_y == space_6y && pos_x == space_6x){
                space_ptr = &space_6;
            }else if(pos_y == space_7y && pos_x == space_7x){
                space_ptr = &space_7;
            }else if(pos_y == space_8y && pos_x == space_8x){
                space_ptr = &space_8;
            }else if(pos_y == space_9y && pos_x == space_9x){
                space_ptr = &space_9;
            }
            if(*space_ptr == ' '){
                if(side == 'X'){
                    *space_ptr = 'X';
                }else{
                    *space_ptr = 'O';
                }
                for(cx = sx; cx <= col; cx++){
                    // Clear the info line
                    mvaddch(info_line, cx, ' ');
                }
                slen = strlen(done_splash);
                sx = col / 2 - slen / 2;
                mvprintw(info_line, sx, done_splash);   
                move(pos_y, pos_x);
                refresh();
                inputting = 0;
            }else{
                for(cx = sx; cx <= col; cx++){
                    // Clear the info line
                    mvaddch(info_line, cx, ' ');
                }
                slen = strlen(full_err_splash);
                sx = col / 2 - slen / 2;
                mvprintw(info_line, sx, full_err_splash);   
                move(pos_y, pos_x);
            }
        }else{
            // If the user presses any other button 
            for(cx = sx; cx <= col; cx++){
                // Clear the info line
                mvaddch(info_line, cx, ' ');
            }
            slen = strlen(input_err_splash);
            sx = col / 2 - slen / 2;
            mvprintw(info_line, sx, input_err_splash);
            move(pos_y, pos_x);
        }
    }
}

int f_AI_picker(){
    
    int pick;
    pick = rand() % 9 + 1;
    int order;                                      
    order = rand() % 2 + 1;
    if(space_5 == ' '){
        return 5;
    }else{
        if(order == 1){
            if(space_1 == ' '){
                return 1;
            }else if(space_2 == ' '){
                return 2;
            }else if(space_3 == ' '){
                return 3;
            }else if(space_4 == ' '){
                return 4;
            }else if(space_6 == ' '){
                return 6;
            }else if(space_7 == ' '){
                return 7;
            }else if(space_8 == ' '){
                return 8;
            }else if(space_9 == ' '){
                return 9;
            }
        }else if(order == 2){
            if(space_9 == ' '){
                return 9;
            }else if(space_8 == ' '){
                return 8;
            }else if(space_7 == ' '){
                return 7;
            }else if(space_6 == ' '){
                return 6;
            }else if(space_4 == ' '){
                return 4;
            }else if(space_3 == ' '){
                return 3;
            }else if(space_2 == ' '){
                return 2;
            }else if(space_1 == ' '){
                return 1;
            }
        }
    }
}

void f_AI_turn(){                 
   
    char AI_char;
    if(side == 'X'){
        AI_char = 'O';
    }else{
        AI_char = 'X';
    }
    int space_to_place;
    space_to_place = f_AI_picker();
    if(space_to_place == 1){
        space_1 = AI_char;
    }else if(space_to_place == 2){
        space_2 = AI_char;
    }else if(space_to_place == 3){
        space_3 = AI_char;
    }else if(space_to_place == 4){
        space_4 = AI_char;
    }else if(space_to_place == 5){
        space_5 = AI_char;
    }else if(space_to_place == 6){
        space_6 = AI_char;
    }else if(space_to_place == 7){
        space_7 = AI_char;
    }else if(space_to_place == 8){
        space_8 = AI_char;
    }else if(space_to_place == 9){
        space_9 = AI_char;
    }
    f_paint();
    refresh();  
}

void f_declare_winner(char symbol){           
    
    char *x_wins = "  X is the winner!  ";
    char *o_wins = "  O is the winner!  ";
    char *tie_game = " The game is a tie! ";
    char padding[] = "                   ";
    char *win_splash_ptr = NULL;
    // Paint background for victory splash:
    if(symbol == 'X'){
      win_splash_ptr = x_wins;
      attron(COLOR_PAIR(Xs));
        for(y = 0; y <= row; y++){
            for(x = 0; x <= col; x++){
                if(x == 0 || x % 2 == 0){
                    mvaddch(y, x, 'X'); 
                }else{
                    mvaddch(y, x, ' ');
                }
            }
        }
      attroff(COLOR_PAIR(Xs));
    }else if(symbol == 'O'){
        win_splash_ptr = o_wins;
      attron(COLOR_PAIR(Os));
        for(y = 0; y <= row; y++){
            for(x = 0; x <= col; x++){
                if(x == 0 || x % 2 == 0){
                    mvaddch(y, x, 'O'); 
                }else{
                mvaddch(y, x, ' ');
                }
            }
        }
      attroff(COLOR_PAIR(Os));
    }else if(symbol == 'T'){
        win_splash_ptr = tie_game;
        for(y = 0; y <= row; y++){
            for(x = 0; x <= col; x++){
                if(x == 0 || x % 2 == 0){
                    attron(COLOR_PAIR(Xs));
                    mvaddch(y, x, 'X'); 
                    attroff(COLOR_PAIR(Xs));
                }else{
                    attron(COLOR_PAIR(Os));
                    mvaddch(y, x, 'O');
                    attroff(COLOR_PAIR(Os));
                }
            }
        }
    }
    //Paint the prompt
    y = row / 2 - 2;
    slen = strlen(win_splash_ptr);
    x = col / 2 - slen / 2;
    mvprintw(y++, x, padding);
    mvprintw(y++, x, win_splash_ptr);
    mvprintw(y, x, padding);
    curs_set(0);
    refresh();
    getch();
    running = 0;
    playing = 0;
}

void f_evaluate_turn(){           
  
    int winner;
    winner = 'N';  // For none
    if(space_1 == 'O' && space_2 == 'O' && space_3 == 'O'){
        winner = 'O';
        game_over++;
    }else if(space_4 == 'O' && space_5 == 'O' && space_6 == 'O'){
        winner = 'O';
        game_over++;
    }else if(space_7 == 'O' && space_8 == 'O' && space_9 == 'O'){
        winner = 'O';
        game_over++;
    }else if(space_1 == 'O' && space_4 == 'O' && space_7 == 'O'){
        winner = 'O';
        game_over++;
    }else if(space_2 == 'O' && space_5 == 'O' && space_8 == 'O'){
        winner = 'O';
        game_over++;
    }else if(space_3 == 'O' && space_6 == 'O' && space_9 == 'O'){
        winner = 'O';
        game_over++;
    }else if(space_1 == 'O' && space_5 == 'O' && space_9 == 'O'){
        winner = 'O';
        game_over++;
    }else if(space_3 == 'O' && space_5 == 'O' && space_7 == 'O'){
        winner = 'O';
        game_over++;
    }else if(space_1 == 'X' && space_2 == 'X' && space_3 == 'X'){
        winner = 'X';
        game_over++;
    }else if(space_4 == 'X' && space_5 == 'X' && space_6 == 'X'){
        winner = 'X';
        game_over++;
    }else if(space_7 == 'X' && space_8 == 'X' && space_9 == 'X'){
        winner = 'X';
        game_over++;
    }else if(space_1 == 'X' && space_4 == 'X' && space_7 == 'X'){
        winner = 'X';
        game_over++;
    }else if(space_2 == 'X' && space_5 == 'X' && space_8 == 'X'){
        winner = 'X';
        game_over++;
    }else if(space_3 == 'X' && space_6 == 'X' && space_9 == 'X'){
        winner = 'X';
        game_over++;
    }else if(space_1 == 'X' && space_5 == 'X' && space_9 == 'X'){
        winner = 'X';
        game_over++;
    }else if(space_3 == 'X' && space_5 == 'X' && space_7 == 'X'){
        winner = 'X';
        game_over++;
    }else if(turn >= 5){
        winner = 'T';
        game_over++;
    }
    if(winner != 'N'){
        f_declare_winner(winner);   
    }
}
