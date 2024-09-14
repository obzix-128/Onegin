#include "SkipLine.h"

const int winEOF = 26;

void skipLine(void)
{
    int ch = 0;
    while (((ch = getchar()) != '\n') && (ch != winEOF) && (ch != EOF))
            ;
}
