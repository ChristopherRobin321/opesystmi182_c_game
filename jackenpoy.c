#include <stdlib.h>      // stdlib needed for getch
#include <ncurses.h>     // ncurses library
/* Internal function declarations   */

int  rock ();
int paper();
int scissors();
int reload();

/* main function */

int main(void) {
 	 initscr();
 	 noecho();
 	 curs_set(FALSE);

 	 rock();
 	 reload();

 	 paper();
 	 reload();

 	 scissors();
	  reload();

	  endwin();
  return 0;
	}

	/* internal functions  */
	int reload()
{
	refresh();
  	getch();
}

int  rock (){
	  mvaddstr(10, 33, "    ***      ");
 	 mvaddstr(10, 33, "   *****     ");
 	 mvaddstr(11, 33, "   ******    ");
 	 mvaddstr(12, 33, "   *****     ");
 	 mvaddstr(13, 33, "    ***      ");
 	 mvaddstr(13, 33, "     *        ");
}

int paper(){
 	 mvaddstr(10, 33, "    ******   ");
 	 mvaddstr(10, 33, "   ******    ");
 	 mvaddstr(11, 33, "   *****     ");
 	 mvaddstr(12, 33, "  *****      ");
 	 mvaddstr(13, 33, " *****       ");
 	 mvaddstr(13, 33, " ****        ");
}

int scissors(){
 	 mvaddstr(10, 33, "  **    **   ");
 	 mvaddstr(10, 33, "   **  **    ");
 	 mvaddstr(11, 33, "    ****     ");
 	 mvaddstr(12, 33, "    ***      ");
 	 mvaddstr(13, 33, "  **   **    ");
 	 mvaddstr(13, 33, "  **   **    ");
}
