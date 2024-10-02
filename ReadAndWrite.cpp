#include "../include/Main.h"
#include "../include/ReadAndWrite.h"
#include "../include/TextHandler.h"


ErrorName readFile(File_Information* eugene_onegin, const char* const file_name, Line_Information** lines_novel)
{
    struct stat file_info = {};

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

    ErrorName check_errors = NO_ERROR;

    eugene_onegin->number_of_lines = countLines(eugene_onegin->text_address, eugene_onegin->text_size);

    *lines_novel = (Line_Information*) calloc(eugene_onegin->number_of_lines, sizeof(Line_Information));
    if(lines_novel == NULL)
    {
        free(eugene_onegin->text_address);
        errorHandler(check_errors);
        return NO_ERROR;
    }

    check_errors = getDataAboutLines(*lines_novel, eugene_onegin->text_address, eugene_onegin->clear_text_novel,
                                     eugene_onegin->text_size);
    if(check_errors != NO_ERROR)
    {
        free(eugene_onegin->text_address);
        free(*lines_novel);
        errorHandler(check_errors);
        return NO_ERROR;
    }

    return NO_ERROR;
}

ErrorName printfResults(Line_Information* lines_novel, const char* const file_name,
                             char* const text_novel, const int count_line, const int size_text_novel)
{
    assert(lines_novel != NULL);
    assert(text_novel  != NULL);
    assert(file_name   != NULL);

    char* file_to_write = (char*) calloc(strlen(file_name) + 6, sizeof(char)); //TODO: const?
    if(file_to_write == NULL)
    {
        return CALLOC_ERROR;
    }
    sprintf(file_to_write, "Sort%s", file_name);

    FILE* eugene_onegin_sort_file = fopen(file_to_write, "w");
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
