#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define DX 3
#define ESCAPE 27

void print_single_line(WINDOW *win, FILE *f, int width) {
    if (feof(f)) {
        return;
    }

    char buf[width];
    memset(buf, 0, width);
    
    fgets(buf, width, f);
    wprintw(win, "%s", buf);
}

void show(const char *filename, FILE *f) {
    initscr();
    noecho();
    cbreak();
    printw("File: %s", filename);

    WINDOW *win = newwin(rows, cols, 2*DX, 2*DX);
    int rows = LINES-2*DX;
    int cols = COLS-2*DX;

    keypad(win, TRUE);
    scrollok(win, TRUE);

    for (int i = 0; i < rows-1; i++) {
        print_single_line(win, f, cols);
    }

    wrefresh(win);
    int c = wgetch(win);
    while (c != ESCAPE) {
        if (c == ' ') {
            print_single_line(win, f, cols);
            wrefresh(win);
        }
        c = wgetch(win);
    }

    endwin();
}


int main(int argc, const char *argv[]) {

    if (argc != 2) {
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        printf("No such file '%s'\n", argv[1]);
        return 2;
    }

    show(argv[1], f);

    fclose(f);

    return 0;
}