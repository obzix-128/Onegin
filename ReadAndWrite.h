#ifndef READANDWRITE_H
#define READANDWRITE_H

#include <string.h>
#include <sys/stat.h>
#include "ErrorHandler.h"


void printfOriginalNovel(char* const text_novel, FILE* eugene_onegin_sort_file, const int size_text_novel);

NumberOfErrors readFile(File_Information* eugene_onegin, const char* const file_name, unsigned long* get_address);
NumberOfErrors printfResults(Line_Information* lines_novel, const char* const file_name,
                             char* const text_novel, const int count_line, const int size_text_novel);

#endif // READANDWRITE_H
