#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

enum ErrorName{
    NO_ERROR      = 0,
    CALLOC_ERROR  = 1,
    STAT_ERROR    = 2,
    FOPEN_ERROR   = 3,
    UNKNOWN_ERROR = 4
};

void errorHandler(ErrorName error);

#endif // ERRORHANDLER_H
