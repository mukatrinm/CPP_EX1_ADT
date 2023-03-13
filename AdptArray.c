#include "AdptArray.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void InitAdptArray(PAdptArray, COPY_FUNC, DEL_FUNC, PRINT_FUNC);
void DeleteDArray(PAdptArray);
Result ResizeArray(PAdptArray adpt_arr, int new_size);

struct AdptArray_ {
    int size;
    PElement *d_array;
    COPY_FUNC copyFunc;
    DEL_FUNC delFunc;
    PRINT_FUNC printFunc;
};

PAdptArray CreateAdptArray(COPY_FUNC copyFunc, DEL_FUNC delFunc, PRINT_FUNC printFunc) {
    PAdptArray adptArray = (PAdptArray) malloc(sizeof(struct AdptArray_));
    if (adptArray == NULL) {
        return NULL;
    }
    
    InitAdptArray(adptArray, copyFunc, delFunc, printFunc);

    return adptArray;
}

void InitAdptArray(PAdptArray adpt_arr, COPY_FUNC copyFunc, DEL_FUNC delFunc, PRINT_FUNC printFunc) {
    if (adpt_arr == NULL) {
        return;
    }
    adpt_arr->size = 0;
    // adpt_arr->d_array = NULL;
    adpt_arr->d_array = calloc(1, sizeof(PElement));

    adpt_arr->copyFunc = copyFunc;
    adpt_arr->delFunc = delFunc;
    adpt_arr->printFunc = printFunc;
}


int GetAdptArraySize(PAdptArray adpt_arr) {
    if (adpt_arr == NULL)
        return -1;

    return adpt_arr->size;
}

PElement GetAdptArrayAt(PAdptArray adpt_arr, int index) {
    if (adpt_arr == NULL) {
        return NULL;
    }
    
    if (index < adpt_arr->size) {
        if (adpt_arr->d_array[index] == NULL) {
            return NULL;
        } else {
            return adpt_arr->copyFunc(adpt_arr->d_array[index]);
        }
    }    

    return NULL;
}

Result SetAdptArrayAt(PAdptArray adpt_arr, int index, PElement element) {
    if (index < adpt_arr->size)
    {
        free(adpt_arr->d_array[index]);
        adpt_arr->d_array[index] = adpt_arr->copyFunc(element);
    }

    // printf("resizing array\n");
    Result result = ResizeArray(adpt_arr, index + 1);
    if (result == FAIL)
    {
        return FAIL;
    }
    
    adpt_arr->d_array[index] = adpt_arr->copyFunc(element);
    // printf("updated value\n");

    return SUCCESS;
}

Result ResizeArray(PAdptArray adpt_arr, int new_size) {
    PElement *new_dynamic_array = calloc(new_size, sizeof(PElement));
    if (new_dynamic_array == NULL) {
        printf("returning fail\n");
        return FAIL;
    }

    memcpy(new_dynamic_array, adpt_arr->d_array, adpt_arr->size * sizeof(PElement));
    // DeleteDArray(adpt_arr); // free old array

    adpt_arr->d_array = new_dynamic_array;
    adpt_arr->size = new_size;

    return SUCCESS;
}

void DeleteAdptArray(PAdptArray adpt_arr) {
    if (adpt_arr == NULL) {
        return;
    }

    DeleteDArray(adpt_arr);

    free(adpt_arr);
    adpt_arr = NULL;
} 

// free the dynamic array
void DeleteDArray(PAdptArray adpt_arr) {
    if (adpt_arr->d_array == NULL) {
        return;
    }

    for (size_t i = 0; i < adpt_arr->size; i++)
    {
        if (adpt_arr->d_array[i]) {
            adpt_arr->delFunc(adpt_arr->d_array[i]);
        // adpt_arr->d_array[i] = NULL; // TOOD: do i need this?
        }
    }
    free(adpt_arr->d_array);
    adpt_arr->d_array = NULL;
} 

void PrintDB(PAdptArray adpt_arr) {
    for (size_t i = 0; i < adpt_arr->size; i++)
    {
        if (adpt_arr->d_array[i])
            adpt_arr->printFunc(adpt_arr->d_array[i]);
    }
}
