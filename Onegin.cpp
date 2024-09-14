#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <stdio.h>

#include "SkipLine.h"

void sortRows(char** line_poem, const int count_line);

// TODO: прочитать про статические переменные

int main(void) // TODO: РАЗБИТЬ НА ФУНКЦИИ
{
    FILE* eugene_onegin_text = fopen("EugeneOnegin.txt", "rb");
    if(eugene_onegin_text == NULL)
    {
        fprintf(stderr, "Error: file not open");
        return 1;
    }

    fseek(eugene_onegin_text, 0, SEEK_END);
    long size_text_poem = ftell(eugene_onegin_text);
    rewind (eugene_onegin_text);

    char* text_poem = (char*) calloc(size_text_poem + 1, sizeof(char));
    text_poem[size_text_poem] = '\0';

    if(text_poem == NULL)
    {
        printf("Error: out of memory"); // TODO: error calloc
        fclose(eugene_onegin_text);
        return 1;
    }

    fread(text_poem, sizeof(char), size_text_poem, eugene_onegin_text);

    int count_line = 1;
    for(int i = 0; i < size_text_poem - 1; i++)
    {
        if(text_poem[i] == '\n') // TODO: \r -> \0 \n -> \0
        {
            count_line++;
        }
    }

    char** line_poem = (char**) calloc(count_line, sizeof(char*));
    line_poem[0] = text_poem;
    for(int i = 0, j = 1; i < size_text_poem - 1; i++)
    {
        if(text_poem[i] == '\n')
        {
            line_poem[j] = (text_poem + i + 1);
            //printf("line_poem[%d] = %d; text_poem[%d + 1] = %c\n", j, line_poem[j], i, text_poem[i + 1]);
            j++;
        }
    }

    sortRows(line_poem, count_line);
    for(int j = 0; j < count_line; j++) // TODO: названия j -> str_number
    {
        printf("%d : ", j + 1);
        for(int i = 0; (*((*(line_poem + j)) + i)) != '\n'; i++)
        {
            printf("%c", (*((*(line_poem + j)) + i))); // TODO: печатать построчно!!!
        }
        printf("\n");
    }

    free(line_poem);
    free(text_poem);
    return 0;
}

void sortRows(char** line_poem, const int count_line)
{
    char* slot = NULL;

    for(int r = 2; r < count_line; r++) // TODO: подумать ещё раз (раз десять...)
    {
        for(int j = 0; j <= count_line - r; j++)
        {
            int skip_not_letter_first = 0, skip_not_letter_second = 0;
            for(int i = 0, k = 0; k == 0; i++)
            {
                printf("|%d||%d|\n", (*((*(line_poem + j)) + i + skip_not_letter_first)), (*((*(line_poem + 1 + j)) + i + skip_not_letter_second)));
                if((isalpha(*((*(line_poem + j)) + i + skip_not_letter_first))) == 0)
                {
                    skip_not_letter_first++;
                    i--;
                    continue;
                }

                if((isalpha(*((*(line_poem + 1 + j)) + i + skip_not_letter_second))) == 0)
                {
                    skip_not_letter_second++;
                    i--;
                    continue;
                }

                printf("{%d}{%d}\n", (*((*(line_poem + j)) + i + skip_not_letter_first)), (*((*(line_poem + 1 + j)) + i + skip_not_letter_second)));
                printf("[%d][%d]\n", skip_not_letter_first, skip_not_letter_second);

                k = (int (tolower(*((*(line_poem + j)) + i + skip_not_letter_first)))) -
                    (int (tolower(*((*(line_poem + 1 + j)) + i + skip_not_letter_second))));

                printf("<%.10s><%.10s>\n", (*(line_poem + j)), (*(line_poem + j + 1)));
                printf("<%c><%c>\n", (*((*(line_poem + j)) + i + skip_not_letter_first)), (*((*(line_poem + 1 + j)) + i + skip_not_letter_second)));

                if(0 < (int (tolower(*((*(line_poem + j)) + i + skip_not_letter_first)))) -
                       (int (tolower(*((*(line_poem + 1 + j)) + i + skip_not_letter_second)))))
                {
                    slot = (*(line_poem + j));
                    line_poem[j] = (*(line_poem + j + 1));
                    line_poem[j + 1] = slot;
                    printf("[%.10s][%.10s]\n", (*(line_poem + j)), (*(line_poem + j + 1)));
                    break;
                }
            }
            printf("j = :%d:\n", j);
        }
        printf("r = :%d:\n", r);
    }
}
