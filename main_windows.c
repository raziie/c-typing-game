#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "helper_windows.h"
char words[10][20];
int line = 0, num_of_word = 0, num_of_current_word = 0, num_of_letter_of_current_word = 0, wave = 0, total_score, score = 0, is_frozen = 0, is_honorary_mode = 0, lives = 3;
char current_word[20], random[3];
float round_time, decrease_percentage;
FILE *data;
void print()
{
    printf("score:%d\t\t", total_score);
    printf("lives:%d\n\n", lives);
    int i, j, spaces, current_line = line, length;
    for (i = 0; i < 20; i++)
    {
        printf("#");
        if ((current_line == random[0] || current_line == random[1] || current_line == random[2]) && current_line > num_of_current_word)
        {
            for (j = 0; j < 20; j++)
                printf("*");
        }
        else if (current_line < 10 && current_line >= 0)
        {
            if (is_honorary_mode == 1)
                length = 3;
            else
                length = strlen(words[current_line]);
            spaces = (20 - length) / 2;
            for (j = 0; j < spaces; j++)
                printf(" ");
            if (current_line == num_of_current_word)
            {
                for (j = 0; j < num_of_letter_of_current_word; j++)
                {
                    printf("\033[0;31m");
                    printf("%c", words[current_line][j]);
                }
                for (j = num_of_letter_of_current_word; j < length; j++)
                {
                    printf("\033[0m");
                    printf("%c", words[current_line][j]);
                }
            }
            else if (current_line > num_of_current_word && strcmp(words[current_line], words[num_of_current_word]) != 0)
            {
                for (j = 0; j < length; j++)
                    printf("%c", words[current_line][j]);
            }
            else
            {
                for (j = 0; j < length; j++)
                    printf(" ");
            }
            printf("\033[0m");
            for (j = 0; j < 20 - (spaces + length); j++)
                printf(" ");
        }
        else
        {
            for (j = 0; j < 20; j++)
                printf(" ");
        }
        printf("#\n");
        current_line--;
    }
    for (i = 1; i <= 22; i++)
        printf("#");
    printf("\n");
    fflush(stdout);
}
void which_type(char *current_word)
{
    int i;
    for (i = 0; i < strlen(current_word); i++)
    {
        if (current_word[i] == '@' || current_word[i] == '$' || current_word[i] == '%' || current_word[i] == '^' || current_word[i] == '&' || current_word[i] == '!')
            score = 3;
        else if (current_word[i] == '_' && score != 3)
            score = 2;
        else if (score != 3 && score != 2)
            score = 1;
    }
}
void frozen(char *current_word)
{
    int i;
    for (i = 0; i < strlen(current_word); i++)
        if (current_word[i] == '#')
        {
            is_frozen = 1;
            break;
        }
}
void my_callback_on_key_arrival(char c)
{
    int length;
    if (c == current_word[num_of_letter_of_current_word] && line < 20 + num_of_current_word)
    {
        num_of_letter_of_current_word++;
        if (is_honorary_mode == 1)
            length = 3;
        else
            length = strlen(current_word);
        if (num_of_letter_of_current_word == length)
        {
            which_type(current_word);
            if (score == 1 && length > 10)
                score = 2;
            if (score == 3 && (line - num_of_current_word + 1 == 18 || line - num_of_current_word + 1 == 19 || line - num_of_current_word + 1 == 20)&&length>10)
                is_honorary_mode = 1;
            printf("%d", (line - num_of_current_word + 1));
            if ((num_of_current_word == random[0] || num_of_current_word == random[1] || num_of_current_word == random[2]))
            {
                score++;
            }
            total_score += score;
            score = 0;
            frozen(current_word);
            num_of_current_word++;
            num_of_letter_of_current_word = 0;
            if(num_of_current_word<10)
            strcpy(current_word, words[num_of_current_word]);
        }
        printf("\033[1;1H\033[2J");
        print();
    }
}
void get_words()
{
    int i;
    for (i = 0; i < 10; i++)
    {
        fscanf(data, "%s", words[i]);
    }
}
void Random()
{
    int i;
    for (i = 0; i < 3; i++)
        random[i] = rand() % 10;
}
long long int main()
{
    int level, you_won = 0, times_up = 0, i;
    printf("select your difficulty:\n");
    printf("1.easy\n");
    printf("2.medium\n");
    printf("3.difficult\n");
    scanf("%d", &level);
    switch (level)
    {
    case 1:
    {
        round_time = 10;
        decrease_percentage = 20;
        break;
    }
    case 2:
    {
        round_time = 8;
        decrease_percentage = 30;
        break;
    }
    case 3:
    {
        round_time = 5;
        decrease_percentage = 40;
        break;
    }
    }
    setvbuf(stdout, NULL, _IOFBF, 500);
    printf("\033[1;1H\033[2J");
    data = fopen("data.txt", "r");
    HANDLE thread_id = start_listening(my_callback_on_key_arrival);
    while (round_time >= 1)
    {
        get_words();
        Random();
        strcpy(current_word, words[num_of_current_word]);
        while (line < 20 + num_of_current_word)
        {
            printf("\033[1;1H\033[2J");
            print();
            if (is_honorary_mode == 1)
            {
                for (i = 100; i <= 2000 * round_time; i += 100)
                {
                    Sleep(100);
                    if (line - num_of_current_word + 1 == 21)
                        break;
                }
                is_honorary_mode = 0;
            }
            if (is_frozen == 1)
            {
                Sleep(3000 * round_time);
                is_frozen = 0;
            }
            if (num_of_current_word > 9)
            {
                printf("\033[1;1H\033[2J");
                break;
            }
            Sleep(round_time * 1000);
            line++;
        }
        if (num_of_current_word != 10)
        {
            if (lives > 1)
            {
                lives--;
                wave--;
            }
            else
            {
                lives--;
                printf("\033[1;1H\033[2J");
                print();
                printf("game over :(\n");
                printf("wave's number:%d\nyour score:%d\n", wave + 1, total_score);
                switch (level)
                {
                case 1:
                {
                    printf("difficulty:easy");
                    break;
                }
                case 2:
                {
                    printf("difficulty:medium");
                    break;
                }
                case 3:
                {
                    printf("difficulty:difficult");
                    break;
                }
                }
                you_won = 1;
                fflush(stdout);
                Sleep(5000);
                break;
            }
        }
        if (round_time == 1)
            times_up = 1;
        round_time = round_time * (1 - (decrease_percentage / 100));
        if (times_up == 0 && round_time < 1)
        {
            round_time = 1;
            times_up = 1;
        }
        wave++;
        num_of_current_word = 0;
        num_of_letter_of_current_word = 0;
        line = 0;
    }
    if (you_won == 0)
    {
        printf("your typing speed is good!\n");
        printf("your score:%d", total_score);
        fflush(stdout);
        Sleep(5000);
    }
    fclose(data);
    WaitForSingleObject(thread_id, INFINITE);
    return 0;
}