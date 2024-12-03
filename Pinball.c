#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define width 20
#define height 20

int dir_x = 1;
int dir_y = 1;
int flip_down;
int flip_up;
int flip_size = 3;
char player1_name[50];
char player2_name[50];
int highscore = 0;
int score1 = 0, score2 = 0;
char highscore_name[50] = "None";

char board[width][height];
int x = width / 2, y = 1;
void resetGame()
{
    score1 = 0;
    score2 = 0;
    dir_x = 1;
    dir_y = 1;
}

void initialize_board()
{
    srand(time(NULL));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            board[i][j] = ' ';
        }
    }
    flip_down = (width / 2) - 2;
    for (int i = flip_down; i <= flip_down + 3; i++)
    {
        board[height - 1][i] = '_';
    }

    flip_up = (width / 2) - 2;
    for (int i = flip_up; i <= (flip_up + 3); i++)
    {
        board[0][i] = '_';
    }

    x = width / 2;
    y = 1;
    board[y][x] = 'O';

    for (int i = 0; i < 5; i++)
    {
        int item_x = rand() % (width - 2) + 1;
        int item_y = rand() % (height - 2) + 1;
        board[item_y][item_x] = '*';
    }
}
void loadHighScore() {
    FILE *file = fopen("highscore.txt", "r");
    if (file != NULL) {
        if (fscanf(file, "%49s %d", highscore_name, &highscore) != 2) {
            strcpy(highscore_name, "None");
            highscore = 0;
        }
        fclose(file);
    } 
}
void saveHighScore(int score, const char *name) {
    loadHighScore();
    if (score > highscore) {
        FILE *file = fopen("highscore.txt", "w");
        if (file != NULL) {
            fprintf(file, "%s %d\n", name, score);
            fclose(file);
        }
        strcpy(highscore_name, name);
        highscore = score;
    }
}



void displayWinner()
{
    system("cls");
    printf("\n**************************************************\n");
    printf("*                                                *\n");
    if (score1 > score2) {
        printf("*         Winner: %-10s                     *\n", player1_name);
        printf("*         Final Score: %-4d                      *\n", score1);
    } else if (score2 > score1) {
        printf("*         Winner: %-10s                     *\n", player2_name);
        printf("*         Final Score: %-4d                      *\n", score2);
    } else {
        printf("*            It's a Tie!                         *\n");
        printf("*         Both Players Scored %-4d               *\n", score1);
    }
    printf("*                                                *\n");
    printf("**************************************************\n\n");
}
void displayHighScore() {
    loadHighScore();
    system("cls");
    loadHighScore();
    printf("\n**************************************************\n");
    printf("*                                                *\n");
    printf("*                High Score                      *\n");
    printf("*                                                *\n");
    printf("*           Player: %-10s                   *\n", highscore_name);
    printf("*           Score : %-4d                         *\n", highscore);
    printf("*                                                *\n");
    printf("**************************************************\n\n");
}

void displayBoard() {
    system("cls");
    printf("******************** PINBALL GAME ********************\n\n");

    printf("+");
    for (int i = 0; i < width; i++)
        printf("-");
    printf("+\n");

    for (int i = 0; i < height; i++) {
        printf("|");
        for (int j = 0; j < width; j++)
            printf("%c", board[i][j]);
        printf("|\n");
    }

    printf("+");
    for (int i = 0; i < width; i++)
        printf("-");
    printf("+\n");
    
    

    printf("\nPlayer 1: %-15s  Score: %d\n", player1_name, score1);
    printf("Player 2: %-15s  Score: %d\n", player2_name, score2);

    saveHighScore(score1,player1_name);
    saveHighScore(score2,player2_name);
    printf("High Score: %-15s  %d\n", highscore_name, highscore);
}
bool updateBall()
{
    static int previous_flip;
    board[y][x] = ' ';
    x += dir_x;
    y += dir_y;

    if (x <= 0 || x >= width - 1)
    {
        dir_x = -dir_x;
    }

    if (y == height - 1 && x >= flip_down && x <= flip_down + 3)
    {
        dir_y = -dir_y;
        score1 += 100;
        previous_flip = 1;
    }
    if (y == 1 && x >= flip_up && x <= (flip_up + 3))
    {
        dir_y = -dir_y;
        score2 += 100;
        previous_flip = 2;
    }

    if (y >= height || y <= 0)
    {
        printf("Game Over! You missed the ball.\n");
        saveHighScore(score1, player1_name);
        saveHighScore(score2, player2_name);
        displayWinner();
        return false;
    }

    if (board[y][x] == '*')
    {
        board[y][x] = ' ';
        if (previous_flip == 2)
        {
            score2 += 50;
            printf("\nprevious = %d\n", previous_flip);
        }
        else if (previous_flip == 1)
        {
            score1 += 50;
            printf("\nprevious = %d\n", previous_flip);
        }
    }

    board[y][x] = 'O';
    for (int i = flip_down; i <= flip_down + 3; i++)
    {
        board[height - 1][i] = '_';
    }
    for (int i = flip_up; i <= flip_up + 3; i++)
    {
        board[0][i] = '_';
    }
}
void handleinput()
{
    if (_kbhit())
    {
        char n = _getch();

        if (n == 'a' && flip_down > 0)
        {
            for (int i = flip_down; i <= (flip_down + 3); i++)
            {
                board[height - 1][i] = ' ';
            }
            flip_down--;

            for (int i = flip_down; i <= (flip_down + 2); i++)
            {
                board[height - 1][i] = '_';
            }
        }
        else if (n == 'd' && (flip_down + 3) < width - 1)
        {
            for (int i = flip_down; i <= (flip_down + 3); i++)
            {
                board[height - 1][i] = ' ';
            }
            flip_down++;
            for (int i = flip_down; i <= (flip_down + 3); i++)
            {
                board[height - 1][i] = '_';
            }
        }

        if (n == 'j' && flip_up > 0)
        {
            for (int i = flip_up; i <= (flip_up + 3); i++)
            {
                board[0][i] = ' ';
            }
            flip_up--;

            for (int i = flip_up; i <= (flip_up + 3); i++)
            {
                board[0][i] = '_';
            }
        }
        else if (n == 'l' && (flip_up + 3) < width - 1)
        {
            for (int i = flip_up; i <= (flip_up + 3); i++)
            {
                board[0][i] = ' ';
            }
            flip_up++;

            for (int i = flip_up; i <= (flip_up + 3); i++)
            {
                board[0][i] = '_';
            }
        }
    }
}
void enterPlayerNames() {
    system("cls");
    printf("\n**************************************************\n");
    printf("*                                                *\n");
    printf("*          ENTER PLAYER NAMES TO BEGIN           *\n");
    printf("*                                                *\n");
    printf("**************************************************\n");
    printf("\nPlayer 1 Name: ");
    fgets(player1_name, sizeof(player1_name), stdin);
    player1_name[strcspn(player1_name, "\n")] = '\0';

    printf("\nPlayer 2 Name: ");
    fgets(player2_name, sizeof(player2_name), stdin);
    player2_name[strcspn(player2_name, "\n")] = '\0';
		printf("player 1 keys are (j,l) flipper up\nplayer 2 keys are (a,d) flipper down\n");
    printf("\nThank you! Let the game begin!\n");
    Sleep(3000);
}
void displayExitMessage() {
    system("cls");
    printf("\n**************************************************\n");
    printf("*                                                *\n");
    printf("*             THANK YOU FOR PLAYING              *\n");
    printf("*                                                *\n");
    printf("*          Goodbye,! See you soon!               *\n");
    printf("*                                                *\n");
    printf("*       MADE BY SOOFIYAN IMRAN & MAAZ IQBAL      *\n");
    printf("*                                                *\n");
    printf("**************************************************\n\n");
}
int main()
{
    int choice;
    do
    {
        system("cls");

        printf("\n");
        printf("**************************************************\n");
        printf("*                                                *\n");
        printf("*               WELCOME TO THE GAME              *\n");
        printf("*                                                *\n");
        printf("**************************************************\n");
        printf("*                                                *\n");
        printf("*   1. Start Game                                *\n");
        printf("*   2. View High Score                           *\n");
        printf("*   3. Exit                                      *\n");
        printf("*                                                *\n");
        printf("**************************************************\n");
        printf("\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        fflush(stdin);
        switch (choice)
        {
        case 1:
            enterPlayerNames();
            resetGame();
            initialize_board();
            while (updateBall() == true)
            {
                displayBoard();
                updateBall();
                handleinput();
                Sleep(100);
            }
            break;
        case 2:
            displayHighScore();
            break;
        case 3:
            displayExitMessage();
            break;
        default:
            printf("\nInvalid choice! Please try again.\n");
            break;
        }

        printf("\nPress Enter to continue...");
        getchar();
    } while (choice != 3);
}