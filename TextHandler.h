#ifndef TEXTHANDLER_H
#define TEXTHANDLER_H

#include <ctype.h>
#include "Errorhandler.h"

int countLines(char* const text_novel, const int size_text_novel);

NumberOfErrors getDataAboutLines(Line_Information* lines_novel, char* const text_novel, char* clear_text_novel,
                                 const int size_text_novel);

#endif // TEXTHANDLER_H
