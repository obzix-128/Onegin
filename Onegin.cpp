#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <stdio.h>
#include <assert.h>

typedef int(*compare_func_t)(void* a, void* b);

struct File_Information{
    FILE* file_address;
    long  text_size;
    char* text_address;
    int   number_of_lines;
    char* clear_text_roman;
};

struct Line_Information{
    char* line_address;
    char* clear_line_address;
    int   size_clear_line;
};

enum NumberOfErrors{
    NO_ERROR     = 0,
    CALLOC_ERROR = 1,
    STAT_ERROR   = 2,
    READ_ERROR   = 3
};

int compareRows(void* first_struct, void* second_struct);
int countLines(char* const text_roman, const int size_text_roman);
void mySort(void* data, const int number_comparisons, size_t el_size, compare_func_t comparator);
NumberOfErrors readFile(File_Information* eugene_onegin);
NumberOfErrors printfResults(Line_Information* lines_roman, const int count_line);
NumberOfErrors printfOriginalPoem(char* const text_roman, int size_text_roman);
NumberOfErrors getDataAboutLines(char* text_roman, char* clear_text_roman, Line_Information* lines_roman,
                                 int size_text_roman);


int main(void)
{
    NumberOfErrors check_errors = NO_ERROR;
    File_Information eugene_onegin = {0, 0, 0, 0};

    check_errors = readFile(&eugene_onegin);
    if(check_errors != NO_ERROR)
    {
        fprintf(stderr, "Error: reading fail");
        return READ_ERROR;
    }

    eugene_onegin.number_of_lines = countLines(eugene_onegin.text_address, eugene_onegin.text_size);

    Line_Information* lines_roman = (Line_Information*) calloc(eugene_onegin.number_of_lines,
                                                               sizeof(Line_Information));
    if(lines_roman == NULL)
    {
        free(eugene_onegin.text_address);
        fprintf(stderr, "Error: calloc");
        return CALLOC_ERROR;
    }

    check_errors = getDataAboutLines(eugene_onegin.text_address, eugene_onegin.clear_text_roman,
                                     lines_roman, eugene_onegin.text_size);
    if(check_errors != NO_ERROR)
    {
        fprintf(stderr, "Error: calloc");
        free(eugene_onegin.text_address);
        free(lines_roman);
        return CALLOC_ERROR;
    }

    const int number_comparisons = eugene_onegin.number_of_lines - 1;
    mySort(lines_roman, number_comparisons, sizeof(Line_Information), compareRows);

    check_errors = printfResults(lines_roman, eugene_onegin.number_of_lines);
    if(check_errors != NO_ERROR)
    {
        printf("Error: reading fail");
        free(eugene_onegin.clear_text_roman);
        free(lines_roman);
        free(eugene_onegin.text_address);
        return READ_ERROR;
    }

    check_errors = printfOriginalPoem(eugene_onegin.text_address, eugene_onegin.text_size);
    if(check_errors != NO_ERROR)
    {
        printf("Error: reading fail");
        free(eugene_onegin.clear_text_roman);
        free(lines_roman);
        free(eugene_onegin.text_address);
        return READ_ERROR;
    }

    free(eugene_onegin.clear_text_roman);
    free(lines_roman);
    free(eugene_onegin.text_address);

    return NO_ERROR;
}

NumberOfErrors readFile(File_Information* eugene_onegin)
{
    struct stat file_info;

    if(stat("EugeneOnegin.txt", &file_info) == -1)
    {
        return STAT_ERROR;
    }
    eugene_onegin->text_size = file_info.st_size;

    eugene_onegin->file_address = fopen("EugeneOnegin.txt", "rb");
    if(eugene_onegin->file_address == NULL)
    {
        fprintf(stderr, "Error: file not open");
        return READ_ERROR;
    }

    eugene_onegin->text_address = (char* const) calloc(eugene_onegin->text_size + 1, sizeof(char));

    if(eugene_onegin->text_address == NULL)
    {
        fprintf(stderr, "Error: calloc");
        fclose(eugene_onegin->file_address);
        return CALLOC_ERROR;
    }
    eugene_onegin->text_address[eugene_onegin->text_size] = '\0';

    fread(eugene_onegin->text_address, sizeof(char), eugene_onegin->text_size, eugene_onegin->file_address);

    fclose(eugene_onegin->file_address);

    return NO_ERROR;
}

NumberOfErrors printfOriginalPoem(char* const text_roman, int size_text_roman)
{
    assert(text_roman != NULL);

    for(int i = 0; i < size_text_roman - 1; i++)
    {
        if(text_roman[i] == '\0')
        {
            if(text_roman[i + 1] == '\0')
            {
                text_roman[i]     = '\r';
                text_roman[i + 1] = '\n';
                continue;
            }
            text_roman[i + 1] = '\n';
        }
    }

    FILE* eugene_onegin_sort_file = fopen("EugeneOneginSort.txt", "ab");
    if(eugene_onegin_sort_file == NULL)
    {
        return READ_ERROR;
    }

    fprintf(eugene_onegin_sort_file, "\nOriginal:\n\n");
    fprintf(eugene_onegin_sort_file, "%s", text_roman);

    fclose(eugene_onegin_sort_file);

    return NO_ERROR;
}

void mySort(void* data, const int number_comparisons, size_t el_size, compare_func_t comporator)
{
    assert(data != NULL);
    assert(el_size != 0);
    assert(comporator != NULL);

    //printf("(%s)(%s)\n", ((Line_Information*)data)[0].line_address, ((Line_Information*)data)[0].clear_line_address);

    char one_byte = 0;

    for(int i = 0; i < number_comparisons; i++)
    {
        for(int j = 0; j < number_comparisons - i; j++)
        {
            //printf("befor :(%s)(%s)\n", ((Line_Information*)data)[j].clear_line_address, ((Line_Information*)data)[j + 1].clear_line_address);
            if((*comporator)((char*)data + j * el_size, (char*)data +(j + 1) * el_size))
            {
                for(unsigned int r = 0; r < el_size; r++)
                {
                    one_byte = ((char*)data + j * el_size)[r];
                    ((char*)data + j * el_size)[r] = ((char*)data + (j + 1) * el_size)[r];
                    ((char*)data + (j + 1) * el_size)[r] = one_byte; // TODO: ПРЕКРАТИТЬ ХУЙНЮ
                }
            }
            //printf("after :(%s)(%s)\n", ((Line_Information*)data)[j].clear_line_address, ((Line_Information*)data)[j + 1].clear_line_address);
        }
    }
}

int compareRows(void* first_struct, void* second_struct)
{
    assert(first_struct  != NULL);
    assert(second_struct != NULL);

    //printf("sorts :|%s||%s|\n", (((Line_Information*)first_struct)[0].clear_line_address), (((Line_Information*)second_struct)[0].clear_line_address));

    char* first_line  = (((Line_Information*)first_struct )->clear_line_address);
    char* second_line = (((Line_Information*)second_struct)->clear_line_address);

    int size_first_line  = (((Line_Information*)first_struct )->size_clear_line) - 1;
    int size_second_line = (((Line_Information*)second_struct)->size_clear_line) - 1;

    int step_back = 0;
    while(first_line[size_first_line - step_back] - second_line[size_second_line - step_back] == 0)
    {
        if(first_line[size_first_line - step_back] == '\0')
        {
            return 1;
        }
        if(second_line[size_second_line - step_back] == '\0')
        {
            return 0;
        }
        step_back++;
    }

    if(first_line[size_first_line - step_back] - second_line[size_second_line - step_back] < 0)
    {
        return 0;
    }
    return 1;
}

NumberOfErrors printfResults(Line_Information* lines_roman, const int count_line)
{
    assert(lines_roman != NULL);

    FILE* eugene_onegin_sort_file = fopen("EugeneOneginSort.txt", "w");
    if(eugene_onegin_sort_file == NULL)
    {
        fprintf(stderr, "Error: file not open");
        return READ_ERROR;
    }

    fprintf(eugene_onegin_sort_file, "Sort:\n\n");
    for(int j = 0; j < count_line; j++)
    {
        fprintf(eugene_onegin_sort_file, "%s\n", lines_roman[j].line_address);
    }
    fclose(eugene_onegin_sort_file);
    return NO_ERROR;
}

NumberOfErrors getDataAboutLines(char* text_roman, char* clear_text_roman,
                                 Line_Information* lines_roman, int size_text_roman)
{
    assert(text_roman  != NULL);
    assert(lines_roman != NULL);

    int line_counter = 1;
    for(int i = 0, r = 0; i < size_text_roman; i++, r++)
    {
        (lines_roman[r]).line_address = &text_roman[i];
        line_counter++;

        while(text_roman[i] != '\0')
        {
            i++;
        }
        if(text_roman[i + 1] == '\0')
        {
            i++;
        }
    }

    int clear_text_size = line_counter;
    for(int i = 0; i < size_text_roman; i++)
    {
        if(isalpha(text_roman[i]))
        {
            clear_text_size++;
        }
    }

    clear_text_roman = (char*) calloc(clear_text_size, sizeof(char));
    if(clear_text_roman == NULL)
    {
        return CALLOC_ERROR;
    }

    for(int i = 0, line_number = 0, r = 0;
        i < size_text_roman;
        i++, line_number++)
    {
        lines_roman[line_number].clear_line_address = &clear_text_roman[r];

        int size_clear_line = 0;
        while(text_roman[i] != '\0')
        {
            if(isalpha(text_roman[i]))
            {
                clear_text_roman[r] = (char)tolower(text_roman[i]);
                size_clear_line++;
                r++;
            }
            i++;
        }

        if(text_roman[i + 1] == '\0')
        {
            i++;
        }
        clear_text_roman[r + 1] = '\0';
        r++;
        lines_roman[line_number].size_clear_line = size_clear_line;
    //printf("{%s}{%s}\n",lines_roman[n].line_address, lines_roman[n].clear_line_address);
    }
    return NO_ERROR;
}

int countLines(char* const text_roman, const int size_text_roman)
{
    assert(text_roman != NULL);

    int count_line = 0;
    for(int i = 0; i < size_text_roman; i++)
    {
        if(text_roman[i] == '\r')
        {
            text_roman[i] = '\0';
        }

        if(text_roman[i] == '\n')
        {
            text_roman[i] = '\0';
            count_line++;
        }
    }
    return count_line;
}
