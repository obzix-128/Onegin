#include "Main.h"
#include "MySort.h"


void mySort(void* data, const int number_comparisons, const size_t el_size, compare_func_t comporator)
{
    assert(data != NULL);
    assert(el_size != 0);
    assert(comporator != NULL);

    //printf("(%s)(%s)\n", ((Line_Information*)data)[0].line_address, ((Line_Information*)data)[0].clear_line_address);

    for(int i = 0; i < number_comparisons; i++)
    {
        for(int j = 0; j < number_comparisons - i; j++)
        {
            //printf("befor :[%p](%s) [%p](%s)\n", ((Line_Information*)data)[j].clear_line_address, ((Line_Information*)data)[j].clear_line_address, ((Line_Information*)data)[j + 1].clear_line_address, ((Line_Information*)data)[j + 1].clear_line_address);
            if((*comporator)((char*)data + j * el_size, (char*)data +(j + 1) * el_size))
            {
                swapPointerToVoid((void*)((char*)data + j * el_size), (void*)((char*)data + (j + 1) * el_size), el_size);
            }
            //printf("after :[%p](%s) [%p](%s)\n\n", ((Line_Information*)data)[j].clear_line_address, ((Line_Information*)data)[j].clear_line_address, ((Line_Information*)data)[j + 1].clear_line_address, ((Line_Information*)data)[j + 1].clear_line_address);
        }
    }
}

void swapPointerToVoid(void* first, void* second, const size_t el_size)
{
    for(unsigned int i = 0; i < el_size; i++)
    {
        char one_byte = ((char*)first)[i];
        ((char*)first)[i] = ((char*)second)[i];
        ((char*)second)[i] = one_byte;
    }
}
