#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define M 25
#define N 80
#define INITIAL_SPEED 800000
#define INITIAL_SPEED_COEFFICIENT 10

void output(int a[M][N], int m, int n, int speed_coefficient);
void new_life(int old_field[M][N], int new_filed[M][N]);
void copy_field(int old_field[M][N], int new_filed[M][N]);
int live_or_not(int x, int y, int old_field[M][N]);
int input(int field[M][N], int n, int m);
int the_end(int new_filed[M][N], int old_field[M][N]);

int main() {
    int old_field[M][N];
    int new_filed[M][N];
    input(old_field, M, N);

    initscr();
    noecho();
    nodelay(stdscr, TRUE);
    curs_set(0);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

    int reading_error = 0;
    if (freopen("/dev/tty", "r", stdin) == NULL) {
        reading_error = 1;
    }
    output(old_field, M, N, INITIAL_SPEED_COEFFICIENT);
    int speed_coefficient = INITIAL_SPEED_COEFFICIENT;
    int flag = 1;
    while (flag) {
        char speed_symbol = getch();
        fflush(stdout);
        if (speed_symbol == 'a' && speed_coefficient < 20) {
            speed_coefficient++;
        } else if (speed_symbol == 'z' && speed_coefficient > 1) {
            speed_coefficient--;
        } else if (speed_symbol == 'q') {
            flag = 0;
        }
        new_life(old_field, new_filed);
        output(new_filed, M, N, speed_coefficient);
        if (the_end(new_filed, old_field) != 1) {
            flag = 0;
        }
        copy_field(old_field, new_filed);
    }
    usleep(INITIAL_SPEED * 2);
    endwin();
    printf("The game is over!!! (((");
    if (reading_error) {
        printf("reading error!");
    }
    return 0;
}

void output(int a[M][N], int m, int n, int speed_coefficient) {
    clear();
    int speed = INITIAL_SPEED / speed_coefficient;
    usleep(speed);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (a[i][j]) {
                attron(A_BOLD | COLOR_PAIR(1));
                printw("*");
            } else {
                attron(COLOR_PAIR(2));
                printw("-");
            }
        }

        printw("\n");
    }
    refresh();
}

void new_life(int old_field[M][N], int new_filed[M][N]) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            new_filed[i][j] = live_or_not(i, j, old_field);
        }
    }
}

int live_or_not(int x, int y, int old_field[M][N]) {
    int sum = 0;
    int res = 0;
    for (int i = x - 1; i < x + 2; i++) {
        for (int j = y - 1; j < y + 2; j++) {
            sum += old_field[(M + i) % M][(N + j) % N];
        }
    }
    sum -= old_field[x][y];
    if (old_field[x][y]) {
        if (sum == 2 || sum == 3) {
            res = 1;
        }
    } else {
        if (sum == 3) {
            res = 1;
        }
    }

    return res;
}

void copy_field(int old_field[M][N], int new_filed[M][N]) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            old_field[i][j] = new_filed[i][j];
        }
    }
}

int input(int field[M][N], int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            char ch = getchar();
            if (ch == '1') {
                field[i][j] = 1;
            } else if (ch == '0') {
                field[i][j] = 0;
            } else {
                j--;
            }
        }
    }
    return 0;
}

int the_end(int new_filed[M][N], int old_field[M][N]) {
    int res = 0;
    int sum = 0;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            sum += new_filed[i][j];
        }
    }
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (old_field[i][j] != new_filed[i][j]) {
                res = 1;
            }
        }
    }

    if (sum == 0) {
        res = 0;
    }
    return res;
}
