#include "../include/Main.h" // TODO: Два enter-a?
#include "../include/ReadAndWrite.h"
#include "../include/Comporators.h"
#include "../include/MySort.h"


int main(const int argc, const char** argv)
{
    if(argv[1][0] != '-')
    {
        printf("To work with the program, select a mode, to "
               "view the list of modes, enter [-h] after the program name.");
        return NO_ERROR;
    }

    if(strcmp(argv[1], "-h") == 0 &&
       strcmp(argv[1], "-f") == 0 &&
       strcmp(argv[1], "-b") == 0)
    {
        printf("[-h] - Help with the program.\n"
               "[-f] - Sorting by line beginnings. After the \"-f\", specify "
               "the name of the file with the text.\n"
               "[-b] - Sorting by line ends. After the \"-b\", specify "
               "the name of the file with the text.\n");
               return NO_ERROR;
    }

    const int ARG_COUNT = 3;

    if(argc != ARG_COUNT)
    {
        printf("To work with the program, select a mode, to "
                "view the list of modes, enter [-h] after the program name.");
        return NO_ERROR;
    }

    ErrorName check_errors = NO_ERROR;
    File_Information eugene_onegin = {};
    Line_Information* lines_novel  = NULL;
    unsigned long get_address = 0;

    if(argv[2] == NULL)
    {
        errorHandler(UNKNOWN_ERROR);
        return UNKNOWN_ERROR;
    }
    check_errors = readFile(&eugene_onegin, argv[2], &get_address);

    if(check_errors != NO_ERROR)
    {
        errorHandler(check_errors);
        return check_errors;
    }
    lines_novel = (Line_Information*) get_address;

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

    check_errors = printfResults(lines_novel, argv[2], eugene_onegin.text_address, eugene_onegin.number_of_lines,
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
