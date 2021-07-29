#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "snake.h"

int i, j, Field[N][M], x, y, Gy, Head, Tail, Game, Frogs, a, b, var, dir, score, HighScore, speed;

FILE *f;

void gotoxy(int x,int y)
{
    COORD coord = {x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

void snake()
{
    // opening 'highscore' file and taking the value from the file and storing it in HighScore variable
    f = fopen("highscore.txt", "r");
    fscanf(f, "%d", &HighScore);
    fclose(f);

    for(i = 0; i < N; i++)
    {
        for(j = 0; j < M; j++)
            Field[i][j] = 0;        // Setting all the values of field to zero
    }

    // setting the starting position of snake to the middle of the field
    x = N/2;
    y = M/2;
    Gy = y;
    Head = 5;
    Tail = 1;
    Game = 0;
    Frogs = 0;
    dir = 'd';          // initializing the direction so at start of the game snake is moving in some direction
    score = 0;
    speed = 100;

    for(i = 0; i < Head; i++)
    {
        Gy++;
        Field[x][Gy - Head] = i + 1;    //setting coordinate for tail and head to appear
    }
}

/*
    ANSI TERMINAL COLORS

    Escape is: \003
    color code is: [0;30m

    to use these colors, we need to use an escape sequence, followed by a correct color code
*/
void flush()            // Resets the text to default color
{
  printf("\033[0m");
}
void black()
{
  printf("\033[1;30m");
}
void red()
{
  printf("\033[0;31m");
}
void bold_red()
{
  printf("\033[1;31m");
}
void green()
{
  printf("\033[0;32m");
}
void bold_green()
{
  printf("\033[1;32m");
}
void yellow()
{
  printf("\033[0;33m");
}
void bold_yellow()
{
  printf("\033[1;33m");
}
void blue()
{
  printf("\033[0;34m");
}
void bold_blue()
{
  printf("\033[1;34m");
}
void magenta()
{
  printf("\033[0;35m");
}
void bold_magenta()
{
  printf("\033[1;35m");
}
void cyan()
{
  printf("\033[0;36m");
}
void bold_cyan()
{
  printf("\033[1;36m");
}
void white()
{
    printf("\033[0;37m");
}



void boundary()//it will print boundary, food and score
{
    for(i = 0; i <= M + 1; i++)//top vertical line of the Field
    {
        if(i == 0)
            printf("%c", 201);  //top left corner
        else if(i == M + 1)
            printf("%c", 187);  //top right corner
        else
            printf("%c", 205);  //vertical line joining top left and top right corner
    }

    printf("Current Score: %d  Highscore: %d  ", score, HighScore);
    printf("\n");
    //will draw two horizontal line from top left corner and top right corner joining bottom left corner and bottom right corner respectively
    for(i = 0; i < N; i++)
    {
        printf("%c", 186);
        for(j = 0; j < M; j++)
        {
            if(Field[i][j] == 0)
                printf(" ");

            if(Field[i][j] > 0 && Field[i][j] != Head)
                printf("%c", 176);      //print tail of the snake
            if(Field[i][j] == Head)
                printf("%c", 178);      //print head of the snake
            //adding a frog on the screen
            if(Field[i][j] == -1)
                printf("%c", 15);      //219

            if(j == M - 1)
                printf("%c\n",186);
        }
    }

    for(i = 0; i <= M + 1; i++)//bottom  vertical line of the Field
    {
        if(i == 0)
            printf("%c",200);   //bottom left corner
        else if(i == M + 1)
            printf("%c", 188);  //bottom right corner
        else
            printf("%c", 205);  //vertical line joining bottom left and bottom right corners
    }

}

void ResetScreenPosition()  // resetting screen
{
    HANDLE hOut;                                //handle allows us to handle the console of the program
    COORD Position;                             //it gives position coordinate of the console screen
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);     //hOut variable(we have created) is used to handle the standard input of the program

    // assigning the topmost corner of the output screen to the Position variable
    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);   //just setting the cursor position

    //gotoxy(0,0);
}

void food()
{
    srand(time(0));

    a = 1 + rand() % 23;
    b = 1 + rand() % 43;

    if(Frogs == 0 && Field[a][b] == 0)  // has the snake eaten the food? and is there space in the field?
    {
        Field[a][b] = -1;   // setting special value to this coordinate so that we can use this while printing the food
        Frogs = 1;          // indicates that food is there on the screen
        if(speed > 10 && score != 0)    // increasing the speed of snake as it eats the food
        {
            speed = speed - 15;          //in this we are decreasing it so that our halt time is less
        }
    }
}

//this function will return the ascii value of the key that is hit
int getch_noblock()
{
    if(_kbhit())
        return _getch();
    else
        return -1;
}

void movement_beginner()
{
    var = getch_noblock();
    var = tolower(var);

    if((var == 'd' || var == 'a' || var == 'w' || var == 's')
        && (abs(dir - var) > 5))         //it checks if the diff b/w asciii values of (w,s) & (a,d) is > 5 so it'll not accept the key opp to the prev key
    {
        dir = var;
    }

    //moves forward
    if(dir == 'd')
    {
        y++;
        if(Field[x][y] != 0 && Field[x][y] != -1)       //if snake's head is not in empty space && it's not on frog
        {
            GameOver();                             //i.e, snake touched it's own body
        }
        if(y == M - 1)        //if it reaches the border
        {
            y = 0;
        }
        if(Field[x][y] == -1)     //if snake's  head is above the frog
        {
            Frogs = 0;          //for creating new food when snake surpasses the food
            score += 10;        //increasing the score when snake eats the food
            Tail -= 1;          //we've defined length of snake from tail pos to head pos
        }                       //like it's from 5->1 and when it moves forward it's from 6->2... 7->3... 8->4
        Head++;
        Field[x][y] = Head;
    }

    //moves backward
    if(dir == 'a')
    {
        y--;
        if(Field[x][y] != 0 && Field[x][y] != -1)
        {
            GameOver();
        }
        if(y == 0)
        {
            y = M - 1;
        }
        if(Field[x][y] == -1)
        {
            Frogs = 0;
            score += 10;
            Tail -= 1;
        }
        Head++;
        Field[x][y] = Head;
    }

    //moves upwards
    if(dir == 'w')
    {
        x--;
        if(Field[x][y] != 0 && Field[x][y] != -1)
        {
            GameOver();
        }
        if(x == -1)
        {
            x = N - 1;
        }
        if(Field[x][y] == -1)
        {
            Frogs = 0;
            score += 10;
            Tail -= 1;
        }
        Head++;
        Field[x][y] = Head;
    }

    //moves downwards
    if(dir == 's')
    {
        x++;
        if(Field[x][y] != 0 && Field[x][y] != -1)
        {
            GameOver();
        }
        if(x == N - 1)
        {
            x = 0;
        }
        if(Field[x][y] == -1)
        {
            Frogs = 0;
            score += 10;
            Tail -= 1;
        }
        Head++;
        Field[x][y] = Head;
    }
}

void movement_easy()
{
    var = getch_noblock();
    var = tolower(var);

    if((var == 'd' || var == 'a' || var == 'w' || var == 's')
       && (abs(dir - var) > 5))
    {
        dir = var;
    }

    //moves forward
    if(dir == 'd')
    {
        y++;
        if(y == M - 1)
        {
            y = 0;
        }
        if(Field[x][y] != 0 && Field[x][y] != -1)
        {
            GameOver();
        }
        if(Field[x][y] == -1)
        {
            Frogs = 0;
            score += 5;
            Tail -= 1;
        }
        Head++;
        Field[x][y] = Head;
    }

    //moves backward
    if(dir == 'a')
    {
        y--;
        if(y == 0)
        {
            y = M - 1;
        }
        if(Field[x][y] != 0 && Field[x][y] != -1)
        {
            GameOver();
        }
        if(Field[x][y] == -1)
        {
            Frogs = 0;
            score += 5;
            Tail -= 1;
        }
        Head++;
        Field[x][y] = Head;
    }

    //moves upwards
    if(dir == 'w')
    {
        x--;
        if(x == -1)
        {
            x = N - 1;
        }
        if(Field[x][y] != 0 && Field[x][y] != -1)
        {
            GameOver();
        }
        if(Field[x][y] == -1)
        {
            Frogs = 0;
            score += 5;
            Tail -= 1;
        }
        Head++;
        Field[x][y] = Head;
    }

    //moves downwards
    if(dir == 's')
    {
        x++;
        if(x == N - 1)
        {
            x = 0;
        }
        if(Field[x][y] != 0 && Field[x][y] != -1)
        {
            GameOver();
        }
        if(Field[x][y] == -1)
        {
            Frogs = 0;
            score += 5;
            Tail -= 1;
        }
        Head++;
        Field[x][y] = Head;
    }
}

void movement_medium()
{
    var = getch_noblock();
    var = tolower(var);

    if((var == 'd' || var == 'a' || var == 'w' || var == 's')
       && (abs(dir - var) > 5))
    {
        dir = var;
    }

    //moves forward
    if(dir == 'd')
    {
        y++;
        if(Field[x][y] != 0 && Field[x][y] != -1)
        {
            GameOver();
        }
        if(y == M)
        {
            GameOver();
        }
        if(Field[x][y] == -1)
        {
            Frogs = 0;
            score += 5;
            Tail -= 1;
        }
        Head++;
        Field[x][y] = Head;
    }

    //moves backward
    if(dir == 'a')
    {
        y--;
        if(Field[x][y] != 0 && Field[x][y] != -1)
        {
            GameOver();
        }
        if(y == -1)
        {
            GameOver();
        }
        if(Field[x][y] == -1)
        {
            Frogs = 0;
            score += 5;
            Tail -= 1;
        }
        Head++;
        Field[x][y] = Head;
    }

    //moves upwards
    if(dir == 'w')
    {
        x--;
        if(Field[x][y] != 0 && Field[x][y] != -1)
        {
            GameOver();
        }
        if(x == -1)
        {
            GameOver();
        }
        if(Field[x][y] == -1)
        {
            Frogs = 0;
            score += 5;
            Tail -= 1;
        }
        Head++;
        Field[x][y] = Head;
    }

    //moves downwards
    if(dir == 's')
    {
        x++;
        if(Field[x][y] != 0 && Field[x][y] != -1)
        {
            GameOver();
        }
        if(x == N)
        {
            GameOver();
        }
        if(Field[x][y] == -1)
        {
            Frogs = 0;
            score += 5;
            Tail -= 1;
        }
        Head++;
        Field[x][y] = Head;
    }
}

void movement_hard()
{
    var = getch_noblock();
    var = tolower(var);

    if((var == 'd' || var == 'a' || var == 'w' || var == 's')
       && (abs(dir - var) > 5))
    {
        dir = var;
    }

    //moves forward
    if(dir == 'd')
    {
        y++;
        if(Field[x][y] != 0 && Field[x][y] != -1)
        {
            GameOver();
        }
        if(y == M)
        {
            GameOver();
        }
        if(Field[x][y] == -1)
        {
            Frogs = 0;
            score += 3;
            Tail -= 1;
        }
        Head++;
        Field[x][y] = Head;
    }

    //moves backward
    if(dir == 'a')
    {
        y--;
        if(Field[x][y] != 0 && Field[x][y] != -1)
        {
            GameOver();
        }
        if(y == -1)
        {
            GameOver();
        }
        if(Field[x][y] == -1)
        {
            Frogs = 0;
            score += 3;
            Tail -= 1;
        }
        Head++;
        Field[x][y] = Head;
    }

    //moves upwards
    if(dir == 'w')
    {
        x--;
        if(Field[x][y] != 0 && Field[x][y] != -1)
        {
            GameOver();
        }
        if(x == -1)
        {
            GameOver();
        }
        if(Field[x][y] == -1)
        {
            Frogs = 0;
            score += 3;
            Tail -= 1;
        }
        Head++;
        Field[x][y] = Head;
    }

    //moves downwards
    if(dir == 's')
    {
        x++;
        if(Field[x][y] != 0 && Field[x][y] != -1)
        {
            GameOver();
        }
        if(x == N)
        {
            GameOver();
        }
        if(Field[x][y] == -1)
        {
            Frogs = 0;
            score += 3;
            Tail -= 1;
        }
        Head++;
        Field[x][y] = Head;
    }
}

void TailRemove()
{
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < M; j++)
        {
            if(Field[i][j] == Tail)     // checking where the value is non zero and setting it to 0
            {
               Field[i][j] = 0;
            }
        }
    }
    //incrementing the tail so that when this function is called again, it checks for different value
    Tail++;
}

void GameOver()
{
    printf("\a");   //beep sound
    Sleep(200);
    system("Cls");

    if(score > HighScore)
    {
        printf("        %d is the NEW HIGHSCORE!!!!!\n\n", score);
        system("pause");
        f = fopen("highscore.txt", "w");        //file opened in write mode
        fprintf(f, "%d", score);                //everything present previously is erased and new highscore gets stored.
        fclose(f);
    }
    system("Cls");

    gotoxy(45,1);
    printf(" GAME OVER !!!!!!!   ");

    gotoxy(45,3);
    printf(" PLAYER SCORE: %d", score);

    gotoxy(45,5);
    printf(" Press Enter to play again or ESC to exit... \n");

    gotoxy(45,5);

    while(1)
    {
        var = getch_noblock(); //gets input from user
        if(var == 13) //enter
        {
            Game = 0;
            system("Cls");
            snake();
            break;
        }
        else if(var == 27) //esc
        {
            Game = 1;
            break;
        }
    }
    system("Cls");
}

void game_details()//controls of the game
{

    system("Cls");

    gotoxy(40, 1);
    //printf(" Welcome !!!!!!!!!!!!!!!!!!!\n");

    bold_yellow();
    gotoxy(45,2);
    //printf("----SNAKE GAME----");
    gotoxy(50,3);
    printf("CONTROLS: ");
    gotoxy(50,4);
    printf("W->UP");
    gotoxy(50,5);
    printf("A->LEFT");
    gotoxy(50,6);
    printf("S->DOWN");
    gotoxy(50,7);
    printf("D->RIGHT");
}

void game_level()//function for selecting game level
{
    bold_red();
    gotoxy(48,12);
    printf("SELECT YOUR LEVEL");
    gotoxy(48,13);
    printf("1)BEGINNER");
    gotoxy(48,14);
    printf("2)EASY");
    gotoxy(48,15);
    printf("3)MEDIUM");
    gotoxy(48,16);
    printf("4)HARD");

    gotoxy(48,17);

    int input;
    scanf("%d",&input);

    switch(input)
    {
        case 1:
        {//case 1 starts
            system("Cls");
            snake();

            while(Game == 0)
            {
                bold_blue();
                boundary();
                ResetScreenPosition();
                food();
                movement_beginner();
                TailRemove();
                Sleep(speed); //this will increase this speed of snake
            }
            break;
        }//case 1 ends

        case 2:
        {//case 2 starts
            system("Cls");
            snake();

            while(Game == 0)
            {
                bold_blue();
                boundary();
                ResetScreenPosition();
                food();
                movement_easy();
                TailRemove();
                Sleep(speed); //this will increase this speed of snake
            }
            break;
        }//case 2 ends

        case 3:
        {//case 3 starts
            system("Cls");
            snake();

            while(Game == 0)
            {
                bold_blue();
                boundary();
                ResetScreenPosition();
                food();
                movement_medium();
                TailRemove();
                Sleep(speed); //this will increase this speed of snake
            }
            break;
        }//case 3 ends

        case 4:
        {//case 4 starts
            system("Cls");
            snake();

            while(Game == 0)
            {
                bold_blue();
                boundary();
                ResetScreenPosition();
                food();
                movement_hard();
                TailRemove();
                Sleep(speed); //this will increase this speed of snake
            }
            break;
        }//case 4 ends

        default:
        {
            gotoxy(50,16);
            printf("WRONG INPUT");
            gotoxy(50,17);
            printf("PRESS ENTER TO GO BACK");
            while(1)
            {
                input = getch_noblock();//taking input
                if(input == 13)//if user press enter the game will run again
                {
                    game_details();
                    game_level();
                    break;
                }
                else if(input == 27)//if user press escape the game will end
                {

                    break;
                }
            }

        }
    }
}

void loading_screen()
{
    int r,q;

    gotoxy(50,10);
    printf(" CLASSIC SNAKE GAME");
    getch();
    system("Cls");

    gotoxy(50,10);
    bold_yellow();
    printf("LOADING.............");
    gotoxy(50,12);
    for(r = 1; r <= 20; r++)
    {
        for(q = 0; q <= 100000000; q++ );
            printf("%c",177);
    }
}

void Highest()
{
    system("cls");
    f = fopen("highscore.txt", "r");
    fscanf(f, "%d", &HighScore);
    gotoxy(50,5);
    printf("Current Highscore is: %d", HighScore);
    fclose(f);
}

void starting_screen()
{
    system("Cls");
    bold_red();

    gotoxy(50,10);
    printf("1)PLAY");
    gotoxy(50,11);
    printf("2)HIGHSCORE");
    gotoxy(50,12);
    printf("3)QUIT");

    gotoxy(50,15);

    int input;
    scanf("%d",&input);

    switch(input)
    {
        case 1:
        {
            game_details();
            game_level();
            break;
        }
        case 2:
        {
            Highest();
            gotoxy(60,30);
            printf("PRESS 'ENTER' TO GO TO THE BACK TO THE MENU");
            while(1)
            {
                input = getch_noblock();//taking input
                if(input == 13)//if user press enter the game will run again
                {
                    system("Cls");
                    starting_screen();
                    break;
                }
                else if(input == 27)//if user press escape the game will end
                {
                    break;
                }
            }
            break;
        }
        case 3:
        {
            system("Cls");
            gotoxy(50,15);
            printf("THANK YOU !!!!!");
            break;
        }
        default:
        {
            system("Cls");
            gotoxy(50,15);
            printf("WRONG INPUT");
            gotoxy(50,16);
            printf("PRESS 'ENTER' TO GO TO MAIN MENU");
            while(1)
            {
                input = getch_noblock();//taking input
                if(input == 13)//if user press enter the game will run again
                {
                    system("Cls");
                    starting_screen();
                    break;
                }
                else if(input == 27)//if user press escape the game will end
                {
                    system("Cls");
                    break;
                }
            }
            break;
        }
    }
}

