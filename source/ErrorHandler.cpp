#include "../include/Main.h"
#include "../include/ErrorHandler.h"


void errorHandler(ErrorName error)
{
    switch(error)
    {
        case NO_ERROR:
        {
            fprintf(stderr, "No errors were found.\n");

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

            break;
        }
        default:
        {
            fprintf(stderr, "Unknown error.\n");

            break;
        }
    }
}
