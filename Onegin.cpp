#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <stdio.h>

#include "SkipLine.h"

typedef int(*compare_func_t)(const void* a, const void* b);

int compareRows(const void* first_row, const void* second_row);
void mySort(void* data, int count_line, size_t el_size, compare_func_t cmpfn);
char* saveOnlyLetters(const void* row, int* size_row);
int countLines(char* text_poem, int size_text_poem);
void findAddressLines(char* text_poem, unsigned long* line_poem, int size_text_poem);
void printfResults(unsigned long* line_poem, int count_line);
void printfPoem(char* text_poem, int size_text_poem);

// TODO: прочитать про статические переменные

int main(void)
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
        printf("Error: calloc");
        fclose(eugene_onegin_text);
        return 1;
    }

    fread(text_poem, sizeof(char), size_text_poem, eugene_onegin_text);

    fclose(eugene_onegin_text);

    int count_line = countLines(text_poem, size_text_poem);

    unsigned long* line_poem = (unsigned long*) calloc(count_line, sizeof(unsigned long));
    if(line_poem == NULL)
    {
        printf("Error: calloc");
        return 1;
    }

    findAddressLines(text_poem, line_poem, size_text_poem);

    int number_comparisons = count_line - 1;
    mySort(line_poem, number_comparisons, sizeof(unsigned long*), compareRows);

    printfResults(line_poem, count_line);

    printfPoem(text_poem, size_text_poem);

    free(line_poem);
    free(text_poem);
    return 0;
}

void printfPoem(char* text_poem, int size_text_poem)
{
    for(int i = 0; i < size_text_poem - 1; i++)
    {
        if(text_poem[i] == '\0')
        {
            if(text_poem[i + 1] == '\0')
            {
                text_poem[i] = '\r';
                text_poem[i + 1] = '\n';
                continue;
            }
            text_poem[i + 1] = '\n';
        }
    }
    printf("%s", text_poem);
}

void mySort(void* data, int number_comparisons, size_t el_size, compare_func_t cmpfn)
{
    //printf("|[%ld]<%s>|\n", *((unsigned long*) data), (char*)*((unsigned long*) data));
    for(int i = 0; i < number_comparisons; i++)
    {
        for(int j = 0; j < number_comparisons - i; j++)
        {
            //printf("<%p><%p>\n", ((const char*)data + j * el_size), ((const char*)data +(j + 1) * el_size));
            if((*cmpfn)((const char*)data + j * el_size, (const char*)data +(j + 1) * el_size))
            {
                unsigned long slot = *((unsigned long*)((char*)data + j * el_size));
                *((unsigned long*)((char*)data + j * el_size)) = *((unsigned long*)((char*)data + (j + 1) * el_size));
                *((unsigned long*)((char*)data + (j + 1) * el_size)) = slot;
            }
        }
    }
}

int compareRows(const void* first_row, const void* second_row)
{
    //printf("|[%p][%p]|\n", ((const char*) first_row), ((const char*) second_row));
    int size_first = 0, size_second = 0;

    char* cleared_first_row = saveOnlyLetters(first_row, &size_first);
    char* cleared_second_row = saveOnlyLetters(second_row, &size_second);

    int flag = 0;
    for(int i = 1; flag == 0; i++)
    {
        if(i > size_first || i > size_second)
        {
            free(cleared_first_row);
            free(cleared_second_row);
            return 0;
        }
        //printf("cleared_first_row[%d] = %c cleared_second_row[%d] = %c\n", i, cleared_first_row[i], i, cleared_second_row[i]);
        flag = (int) cleared_first_row[size_first - i] - (int) cleared_second_row[size_second - i];
    }

    if(flag > 0)
    {
        free(cleared_first_row);
        free(cleared_second_row);
        return 1;
    }

    free(cleared_first_row);
    free(cleared_second_row);
    return 0;
}

char* saveOnlyLetters(const void* row, int* size_row)
{
    char flag = '0';

    for(int i = 0; flag != '\0'; i++)
    {
        //printf("<%c>[%ld]\n", *((const char*)*((const unsigned long*) row) + i), *((((const unsigned long*) row) + i)));
        if(isalpha(*((const char*)*((const unsigned long*) row) + i)))
        {
            (*size_row)++;
        }

        flag = (*((const char*)*((const unsigned long*) row) + i));
    }
    char* cleared_row = (char*) calloc(*size_row + 1, sizeof(char));

    if(cleared_row == NULL)
    {
        printf("Error: calloc");
        return NULL;
    }
    cleared_row[*size_row] = '\0';

    for(int i = 0, j = 0; j < *size_row; i++)
    {
        //printf("<%c>[%ld]\n", *((const char*)*((const unsigned long*) row) + i), *((((const unsigned long*) row) + i)));
        if(isalpha(*((const char*)*((const unsigned long*) row) + i)))
        {
            cleared_row[j] = (char) tolower((*((const char*)*((const unsigned long*) row) + i)));
            j++;
        }
        flag = *((const char*)*((const unsigned long*) row) + i);
    }

    //printf("cleared_row = |%s|\n", cleared_row);
    return cleared_row;
}

void printfResults(unsigned long* line_poem, int count_line)
{
    for(int j = 0; j < count_line; j++)
    {
        printf("%d : %s\n", j + 1, (char*) line_poem[j]);
    }
}

void findAddressLines(char* text_poem, unsigned long* line_poem, int size_text_poem)
{
    line_poem[0] = (unsigned long)&text_poem[0];
    //printf("line_poem[0] = <%ld> \"%s\"; text_poem[0] = %c\n", line_poem[0], (char*)line_poem[0], text_poem[0]);

    for(int i = 0, j = 1; i < size_text_poem - 1; i++)
    {
        if(text_poem[i] == '\0' && text_poem[i + 1] != '\0')
        {
            line_poem[j] = (unsigned long)&text_poem[i + 1];
            //printf("line_poem[%d] = <%ld> \"%s\"; text_poem[%d] = %c\n", j, line_poem[j], (char*)line_poem[j], i + 1, text_poem[i + 1]);
            j++;
        }
    }
}

int countLines(char* text_poem, int size_text_poem)
{
    int count_line = 0;
    for(int i = 0; i < size_text_poem; i++)
    {
        if(text_poem[i] == '\r')
        {
            text_poem[i] = '\0';
        }

        if(text_poem[i] == '\n')
        {
            count_line++;
            text_poem[i] = '\0';
        }
    }
    return count_line;
}
