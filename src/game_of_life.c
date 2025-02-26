#include <stdio.h>
#include <stdlib.h>
//#include <locale.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

int* input(int* count_ptr, const int rows, const int cols);
int** ArrayOfPointersToArray(int rows, const int cols);
void FreeArrayOfPointersToArray(int** data, const int rows);
void FillTheArray(int** data, const int* const array, const int count_ptr);
void MatrixOutput(int** data, const int rows, const int cols);
void Refresh(int*** matrix_ptr, const int rows, const int cols);
int CountNeighbours(int** matrix, const int row, const int col, const int max_row, const int max_col);
void StringOutput(int** data, const int row_number, const int cols);
void ClearField();
void InitializeConcole(struct termios* old, struct termios* new);
bool IfInputAvailable();
void PrintSign();
void PrintRules();
void InitialCoordinatePhrase();
void ReadInput(char* currChar);
int AnalizeInput(char* currChar, int* speed, const int max_speed, const int min_speed);

int main() {
    const int cols = 80;
    const int rows = 25;
    int speed = 10;
    const int min_speed = 4;
    const int max_speed = 30;
    int speed_multiplyer = 10000;
    char currChar = '\n';
    InitialCoordinatePhrase();

    int count = 0;
    int* array = input(&count, rows, cols);

    int tty = open("/dev/tty", O_RDONLY);
    dup2(tty, STDIN_FILENO);

    if (array) {
        int** matrix = ArrayOfPointersToArray(rows, cols);
        FillTheArray(matrix, array, count);

        static struct termios oldt, newt;
        InitializeConcole(&oldt, &newt);

        while (1) {
            ClearField();
            ReadInput(&currChar);
            MatrixOutput(matrix, rows, cols);
            PrintSign();
            PrintRules();
            Refresh(&matrix, rows, cols);
            if (!AnalizeInput(&currChar, &speed, max_speed, min_speed)) {
                break;
            }
            usleep(speed * speed_multiplyer);
        }

        free(array);
        FreeArrayOfPointersToArray(matrix, rows);
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    } else {
        printf("You entered invalid coordinates. Exit.....");
    }

    return 0;
}

int CountNeighbours(int** matrix, const int row, const int col, const int max_row, const int max_col) {
    int counter = 0;
    for (int i = row - 1; i <= row + 1; ++i) {
        for (int j = col - 1; j <= col + 1; ++j) {
            int x_index = (max_row + i) % max_row;
            int y_index = (max_col + j) % max_col;
            if (matrix[x_index][y_index]) {
                ++counter;
            }
        }
    }
    return (matrix[row][col]) ? counter - 1 : counter;
}

void Refresh(int*** matrix_ptr, const int rows, const int cols) {
    int** buffer = ArrayOfPointersToArray(rows, cols);

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int count = CountNeighbours(*matrix_ptr, row, col, rows, cols);

            if ((*matrix_ptr)[row][col] == 0 && count == 3) {
                buffer[row][col] = 1;
            }

            if ((*matrix_ptr)[row][col] == 1 && (count > 3 || count < 2)) {
                buffer[row][col] = 0;
            }

            if ((*matrix_ptr)[row][col] == 1 && (count == 3 || count == 2)) {
                buffer[row][col] = 1;
            }
        }
    }

    int** tmp = *matrix_ptr;
    *matrix_ptr = buffer;
    FreeArrayOfPointersToArray(tmp, rows);
}

void FillTheArray(int** data, const int* const array, const int count_ptr) {
    for (int i = 0; i < count_ptr; i += 2) {
        data[array[i]][array[i + 1]] = 1;
    }
}

void FreeArrayOfPointersToArray(int** data, const int rows) {
    for (int i = 0; i < rows; ++i) {
        free(data[i]);
    }
    free(data);
}

int** ArrayOfPointersToArray(const int rows, const int cols) {
    int** ptr_to_arrays = malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; ++i) {
        ptr_to_arrays[i] = malloc(cols * sizeof(int));
    }
    return ptr_to_arrays;
}

int* input(int* count_ptr, const int rows, const int cols) {
    double x;
    char c;
    int flag = 1;
    int* data = NULL;
    int i = 0;
    data = (int*)calloc(cols * rows, sizeof(int));
    while (flag) {
        flag = scanf("%lf%c", &x, &c);
        if (flag && x == (int)(x) && x >= 0 && ((i % 2 == 0 && x < 25) || (i % 2 == 1 && x < 80))) {
            data[i++] = (int)x;
        } else {
            free(data);
            data = NULL;
            *count_ptr = 0;
            break;
        }
        if (c == '\n') {
            break;
        } else if (c >= '0' && c <= '9') {
            ungetc(c, stdin);
        } else if (c == ' ') {
            continue;
        }
    }
    if (i % 2 == 0)
        *count_ptr = i;
    else {
        *count_ptr = 0;
        free(data);
        data = NULL;
    }
    return data;
}

void MatrixOutput(int** data, const int rows, const int cols) {
    for (int i = 0; i < rows; i++) {
        StringOutput(data, i, cols);
        printf("\n");
    }
}
void StringOutput(int** data, const int row, const int cols) {
    printf("|");
    for (int i = 0; i < cols; i++) {
        if (data[row][i] == 0)
            printf(" |");
        else
            printf("█|");
    }
}

bool IfInputAvailable() {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    return (FD_ISSET(0, &fds));
}

void ClearField() { printf("\e[1;1H\e[2J"); }

void PrintSign() {
    printf("\nDesigned by cinnamoa, meeseekt, and sejuaniw.\nAll rights reserved. (C) 2024.\n\n");
}

void PrintRules() {
    printf("The famous Conway's Game of Life\n");
    printf("Welcome to the terminal 'Game of life' developed by Team_87\n");
    printf("Rules of the game: \n");
    printf(
        "0. The game requires 1 player. Player control keys: 'A' and 'Z' to increase and decrease the "
        "speed.\n");
    printf("1. If you want to stop game, press the space bar, and if you want to exit, press 'Q'.\n");
    printf("2. Any live cell with fewer than two live neighbours dies.\n");
    printf("3. Any live cell with two or three live neighbours lives on to the next generation.\n");
    printf("4. Any live cell with more than three live neighbours dies.\n");
    printf("5. Any dead cell with exactly three live neighbours becomes a live cell.\n");
    printf("   Have a good game!\n\n");
    printf(
        "█▀▀█████████▀▀█\n█▒╖░▀▀▀▀▀▀▀░╓▒█\n█▌┊╓╟╙╕╒╜╢╓┊▐█\n█░╜┌▄╮╜╙╭▄┐╙░█\n▌╜╙░░░░░░░░░╜╙▐\n▌╒╙╓░░╰┬╯░░╜╕▐\n▓"
        "▄╙╓╙╰─┴─╯╜╕▄▓\n▓▓▓██▄▄▄▄▄██▓▓▓\n");
}

void InitialCoordinatePhrase() {
    printf(
        "Please enter coordinates of living cells at the initial state of the field in the following form: "
        "X1 Y1 X2 Y2 X3 ....... ↵.\n");
}

void ReadInput(char* currChar) {
    if (IfInputAvailable()) {
        *currChar = getchar();
    }
}

int AnalizeInput(char* currChar, int* speed, const int max_speed, const int min_speed) {
    int return_value = 1;

    if (*currChar == 'q') {
        return_value = 0;
    }

    if (*currChar == 'a' || *currChar == 'A') {
        *speed -= (*speed > min_speed);
    } else if (*currChar == 'z' || *currChar == 'Z') {
        *speed += (*speed < max_speed);
    } else if (*currChar == ' ') {
        *currChar = '\n';
        while (*currChar != ' ') {
            if (IfInputAvailable()) {
                *currChar = getchar();
            }
        }
    }
    *currChar = '\n';
    return return_value;
}

void InitializeConcole(struct termios* old, struct termios* new) {
    tcgetattr(STDIN_FILENO, old);
    *new = *old;
    new->c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, new);
}
