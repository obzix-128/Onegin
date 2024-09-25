#include "../include/Main.h"
#include "../include/Comporators.h"


int frontRowComparison(const void* first_struct, const void* second_struct)
{
    assert(first_struct  != NULL);
    assert(second_struct != NULL);

    char* first_line  = (((const Line_Information*)first_struct )->clear_line_address);
    char* second_line = (((const Line_Information*)second_struct)->clear_line_address);

    int size_first_line  = (((const Line_Information*)first_struct )->size_clear_line);
    int size_second_line = (((const Line_Information*)second_struct)->size_clear_line);

    int step_front = 0;
    while(first_line[step_front] - second_line[step_front] == 0 &&
          step_front < size_first_line && step_front < size_second_line)
    {
        step_front++;
    }
    return first_line[step_front] - second_line[step_front];
}

int backRowComparison(const void* first_struct, const void* second_struct)
{
    assert(first_struct  != NULL);
    assert(second_struct != NULL);

    //printf("sorts :|%s||%s|\n", (((Line_Information*)first_struct)[0].clear_line_address), (((Line_Information*)second_struct)[0].clear_line_address));

    char* first_line  = (((const Line_Information*)first_struct )->clear_line_address);
    char* second_line = (((const Line_Information*)second_struct)->clear_line_address);

    int size_first_line  = (((const Line_Information*)first_struct )->size_clear_line);
    int size_second_line = (((const Line_Information*)second_struct)->size_clear_line);

    int step_back = 0;
    while(first_line[size_first_line - step_back] - second_line[size_second_line - step_back] == 0 &&
          step_back < size_first_line && step_back < size_second_line)
    {
        step_back++;
    }

    if(first_line[size_first_line - step_back] - second_line[size_second_line - step_back] < 0)
    {
        return 0;
    }
    return 1;
}
