#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

typedef int(*compare_func_t)(const void* a, const void* b);

struct File_Information{
    FILE* file_address;
    char* text_address;
    char* clear_text_novel;
    long  file_size;
    long  text_size;
    int   number_of_lines;
};

struct Line_Information{
    char* line_address;
    char* clear_line_address;
    int   size_clear_line;
};

enum NumberOfErrors{
    NO_ERROR      = 0,
    CALLOC_ERROR  = 1,
    STAT_ERROR    = 2,
    FOPEN_ERROR    = 3,
    UNKNOWN_ERROR = 4
};

int  backRowComparison(const void* first_struct, const void* second_struct);
int frontRowComparison(const void* first_struct, const void* second_struct);
int countLines(char* const text_novel, const int size_text_novel);

void swapPointerToVoid(void* first, void* second, const size_t el_size);
void mySort(void* data, const int number_comparisons, const size_t el_size, compare_func_t comparator);
void printfOriginalNovel(char* const text_novel, FILE* eugene_onegin_sort_file, const int size_text_novel);

NumberOfErrors errorHandler(NumberOfErrors error);
NumberOfErrors readFile(File_Information* eugene_onegin, const char* const file_name);
NumberOfErrors printfResults(Line_Information* lines_novel, char* const text_novel, const int count_line,
                             const int size_text_novel);
NumberOfErrors getDataAboutLines(Line_Information* lines_novel, char* const text_novel, char* clear_text_novel,
                                 const int size_text_novel);


int main(const int argc, const char** argv)
{
    const int MIN_ARG_COUNT = 2;
    const int MAX_ARG_COUNT = 3;

    if(argc < MIN_ARG_COUNT || argc > MAX_ARG_COUNT)
    {
        printf("To work with the program, select a mode, to "
                "view the list of modes, enter [-h] after the program name.");
        return NO_ERROR;
    }

    if(argv[1][1] == 'h')
    {
        printf("[-h] - Help with the program.\n"
               "[-f] - Sorting by line beginnings. After the \"-f\", specify "
               "the name of the file with the text.\n"
               "[-b] - Sorting by line ends. After the \"-b\", specify "
               "the name of the file with the text.\n");
               return NO_ERROR;
    }

    NumberOfErrors check_errors = NO_ERROR;
    File_Information eugene_onegin = {};

    check_errors = readFile(&eugene_onegin, argv[2]);
    if(check_errors != NO_ERROR)
    {
        errorHandler(check_errors);
        return NO_ERROR;
    }

    eugene_onegin.number_of_lines = countLines(eugene_onegin.text_address, eugene_onegin.text_size);

    Line_Information* lines_novel = (Line_Information*) calloc(eugene_onegin.number_of_lines,
                                                               sizeof(Line_Information));
    if(lines_novel == NULL)
    {
        free(eugene_onegin.text_address);
        errorHandler(check_errors);
        return NO_ERROR;
    }

    check_errors = getDataAboutLines(lines_novel, eugene_onegin.text_address, eugene_onegin.clear_text_novel,
                                     eugene_onegin.text_size);
    if(check_errors != NO_ERROR)
    {
        free(eugene_onegin.text_address);
        free(lines_novel);
        errorHandler(check_errors);
        return NO_ERROR;
    }

    if(argv[1][0] != '-')
    {
        printf("To work with the program, select a mode, to "
               "view the list of modes, enter [-h] after the program name.");
        return NO_ERROR;
    }

    switch(argv[1][1])
    {
        case 'f':
        {
            qsort(lines_novel, eugene_onegin.number_of_lines, sizeof(Line_Information), frontRowComparison);

            break;
        }
        case 'b':
        {
            const int number_comparisons = eugene_onegin.number_of_lines - 1;
            mySort(lines_novel, number_comparisons, sizeof(Line_Information), backRowComparison);

            break;
        }
        default:
        {
            printf("To work with the program, select a mode, to view the list of modes, "
                   "enter [-h] after the program name.");

            return NO_ERROR;
            break;
        }
    }

    check_errors = printfResults(lines_novel, eugene_onegin.text_address, eugene_onegin.number_of_lines,
                                 eugene_onegin.text_size);
    if(check_errors != NO_ERROR)
    {
        free(eugene_onegin.clear_text_novel);
        free(lines_novel);
        free(eugene_onegin.text_address);
        errorHandler(check_errors);
        return NO_ERROR;
    }

    free(eugene_onegin.clear_text_novel);
    free(lines_novel);
    free(eugene_onegin.text_address);
    return NO_ERROR;
}

NumberOfErrors errorHandler(NumberOfErrors error)
{
    switch(error)
    {
        case NO_ERROR:
        {
            fprintf(stderr, "Unknown error.\n");

            return UNKNOWN_ERROR;
            break;
        }
        case CALLOC_ERROR:
        {
            fprintf(stderr, "Error: Calloc.\n");

            break;
        }
        case STAT_ERROR:
        {
            fprintf(stderr, "Error: It was not possible to get data about the file.\n");

            break;
        }
        case FOPEN_ERROR:
        {
            fprintf(stderr, "Error: Couldn't open the file.\n");

            break;
        }
        case UNKNOWN_ERROR:
        {
            fprintf(stderr, "Unknown error.\n");

            return NO_ERROR;
            break;
        }
        default:
        {
            fprintf(stderr, "Unknown error.\n");

            return UNKNOWN_ERROR;
            break;
        }
    }

    return NO_ERROR;
}

NumberOfErrors readFile(File_Information* eugene_onegin, const char* const file_name)
{
    struct stat file_info;

    if(stat(file_name, &file_info) == -1)
    {
        return STAT_ERROR;
    }
    eugene_onegin->file_size = file_info.st_size;

    eugene_onegin->file_address = fopen(file_name, "rb");
    if(eugene_onegin->file_address == NULL)
    {
        return FOPEN_ERROR;
    }

    eugene_onegin->text_address = (char* const) calloc(eugene_onegin->file_size + 1, sizeof(char));
    if(eugene_onegin->text_address == NULL)
    {
        fclose(eugene_onegin->file_address);
        return CALLOC_ERROR;
    }

    eugene_onegin->text_size = fread(eugene_onegin->text_address, sizeof(char),
                                     eugene_onegin->file_size, eugene_onegin->file_address);
    eugene_onegin->text_address[eugene_onegin->text_size] = '\0';

    fclose(eugene_onegin->file_address);

    return NO_ERROR;
}

void mySort(void* data, const int number_comparisons, const size_t el_size, compare_func_t comporator)
{
    assert(data != NULL);
    assert(el_size != 0);
    assert(comporator != NULL);

    //printf("(%s)(%s)\n", ((Line_Information*)data)[0].line_address, ((Line_Information*)data)[0].clear_line_address);

    for(int i = 0; i < number_comparisons; i++)
    {
        for(int j = 0; j < number_comparisons - i; j++)
        {
            //printf("befor :[%p](%s) [%p](%s)\n", ((Line_Information*)data)[j].clear_line_address, ((Line_Information*)data)[j].clear_line_address, ((Line_Information*)data)[j + 1].clear_line_address, ((Line_Information*)data)[j + 1].clear_line_address);
            if((*comporator)((char*)data + j * el_size, (char*)data +(j + 1) * el_size))
            {
                swapPointerToVoid((void*)((char*)data + j * el_size), (void*)((char*)data + (j + 1) * el_size), el_size);
            }
            //printf("after :[%p](%s) [%p](%s)\n\n", ((Line_Information*)data)[j].clear_line_address, ((Line_Information*)data)[j].clear_line_address, ((Line_Information*)data)[j + 1].clear_line_address, ((Line_Information*)data)[j + 1].clear_line_address);
        }
    }
}

void swapPointerToVoid(void* first, void* second, const size_t el_size)
{
    for(unsigned int i = 0; i < el_size; i++)
    {
        char one_byte = ((char*)first)[i];
        ((char*)first)[i] = ((char*)second)[i];
        ((char*)second)[i] = one_byte;
    }
}

int frontRowComparison(const void* first_struct, const void* second_struct)
{
    assert(first_struct  != NULL);
    assert(second_struct != NULL);

    char* first_line  = (((const Line_Information*)first_struct )->clear_line_address);
    char* second_line = (((const Line_Information*)second_struct)->clear_line_address);

    int size_first_line  = (((const Line_Information*)first_struct )->size_clear_line);
    int size_second_line = (((const Line_Information*)second_struct)->size_clear_line);

    int step_front = 0;
    while(first_line[step_front] - second_line[step_front] == 0 &&
          step_front < size_first_line && step_front < size_second_line)
    {
        step_front++;
    }
    return first_line[step_front] - second_line[step_front];
}

int backRowComparison(const void* first_struct, const void* second_struct)
{
    assert(first_struct  != NULL);
    assert(second_struct != NULL);

    //printf("sorts :|%s||%s|\n", (((Line_Information*)first_struct)[0].clear_line_address), (((Line_Information*)second_struct)[0].clear_line_address));

    char* first_line  = (((const Line_Information*)first_struct )->clear_line_address);
    char* second_line = (((const Line_Information*)second_struct)->clear_line_address);

    int size_first_line  = (((const Line_Information*)first_struct )->size_clear_line);
    int size_second_line = (((const Line_Information*)second_struct)->size_clear_line);

    int step_back = 0;
    while(first_line[size_first_line - step_back] - second_line[size_second_line - step_back] == 0 &&
          step_back < size_first_line && step_back < size_second_line)
    {
        step_back++;
    }

    if(first_line[size_first_line - step_back] - second_line[size_second_line - step_back] < 0)
    {
        return 0;
    }
    return 1;
}

NumberOfErrors printfResults(Line_Information* lines_novel, char* const text_novel,
                             const int count_line, const int size_text_novel)
{
    assert(lines_novel != NULL);
    assert(text_novel  != NULL);

    const char* FILE_NAME = "SortFile.txt";

    FILE* eugene_onegin_sort_file = fopen(FILE_NAME, "w");
    if(eugene_onegin_sort_file == NULL)
    {
        fprintf(stderr, "Error: file not open");
        return FOPEN_ERROR;
    }

    fprintf(eugene_onegin_sort_file, "Sort:\n\n");
    for(int j = 0; j < count_line; j++)
    {
        fprintf(eugene_onegin_sort_file, "%s\n", lines_novel[j].line_address);
    }

    printfOriginalNovel(text_novel, eugene_onegin_sort_file, size_text_novel);
    fclose(eugene_onegin_sort_file);
    return NO_ERROR;
}

void printfOriginalNovel(char* const text_novel, FILE* eugene_onegin_sort_file,  const int size_text_novel)
{
    assert(text_novel              != NULL);
    assert(eugene_onegin_sort_file != NULL);

    for(int i = 0; i < size_text_novel; i++)
    {
        if(text_novel[i] == '\0')
        {
            if(text_novel[i + 1] == '\0')
            {
                text_novel[i]     = ' ';
                text_novel[i + 1] = '\n';
                i++;
                continue;
            }
            text_novel[i] = '\n';
        }
    }
    text_novel[size_text_novel] = '\0';

    fprintf(eugene_onegin_sort_file, "\nOriginal:\n\n");
    fprintf(eugene_onegin_sort_file, "%s", text_novel);
}

NumberOfErrors getDataAboutLines(Line_Information* lines_novel, char* const text_novel, char* clear_text_novel,
                                 const int size_text_novel)
{
    assert(text_novel       != NULL);
    assert(lines_novel      != NULL);

    int line_counter = 0;
    for(int i = 0, r = 0; i < size_text_novel; i++, r++)
    {
        (lines_novel[r]).line_address = &text_novel[i];
        line_counter++;

        while(text_novel[i] != '\0')
        {
            i++;
        }
        if(text_novel[i + 1] == '\0')
        {
            i++;
        }
    }

    int clear_text_size = line_counter;
    for(int i = 0; i < size_text_novel; i++)
    {
        if(isalpha(text_novel[i]))
        {
            clear_text_size++;
        }
    }

    clear_text_novel = (char*) calloc(clear_text_size, sizeof(char));
    if(clear_text_novel == NULL)
    {
        return CALLOC_ERROR;
    }

    for(int i = 0, line_number = 0, r = 0;
        i < size_text_novel;
        i++, line_number++)
    {
        lines_novel[line_number].clear_line_address = &clear_text_novel[r];

        int size_clear_line = 0;
        while(text_novel[i] != '\0')
        {
            if(isalpha(text_novel[i]))
            {
                clear_text_novel[r] = (char)tolower(text_novel[i]);
                size_clear_line++;
                r++;
            }
            i++;
        }

        if(text_novel[i + 1] == '\0')
        {
            i++;
        }
        clear_text_novel[r + 1] = '\0';
        r++;
        lines_novel[line_number].size_clear_line = size_clear_line;
    }
    return NO_ERROR;
}

int countLines(char* const text_novel, const int size_text_novel)
{
    assert(text_novel != NULL);

    int count_line = 0;

    if(text_novel[size_text_novel - 1] != '\n')
    {
        count_line++;
    }

    for(int i = 0; i < size_text_novel; i++)
    {
        if(text_novel[i] == '\r')
        {
            text_novel[i] = '\0';
        }

        if(text_novel[i] == '\n')
        {
            text_novel[i] = '\0';
            count_line++;
        }
    }
    return count_line;
}
