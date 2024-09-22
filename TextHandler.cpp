#include "Main.h"
#include "TextHandler.h"


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

NumberOfErrors getDataAboutLines(Line_Information* lines_novel, char* const text_novel, char* clear_text_novel,
                                 const int size_text_novel)
{
    assert(text_novel  != NULL);
    assert(lines_novel != NULL);

    int line_counter = 0;
    for(int i = 0, line_number = 0; i < size_text_novel; i++, line_number++)
    {
        (lines_novel[line_number]).line_address = &text_novel[i];
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

    for(int i = 0, line_number = 0, r = 0; //TODO: Изменить функцию? Название переменных?
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

        r++;
        clear_text_novel[r] = '\0';
        lines_novel[line_number].size_clear_line = size_clear_line;
    }
    return NO_ERROR;
}
