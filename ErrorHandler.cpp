#include "Main.h"
#include "ErrorHandler.h"


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
