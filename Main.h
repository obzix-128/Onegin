#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


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

#endif // MAIN_H
