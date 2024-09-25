#ifndef MYSORT_H
#define MYSORT_H

typedef int(*compare_func_t)(const void* a, const void* b);

void mySort(void* data, const int number_comparisons, const size_t el_size, compare_func_t comparator);
void swapPointerToVoid(void* first, void* second, const size_t el_size);

#endif // MYSORT_H
