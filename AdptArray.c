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

/**
 * @brief Create an empty Adpt Array object
 * 
 * @param copyFunc copy function
 * @param delFunc delete function
 * @param printFunc print function
 * @return PAdptArray pointer to the adapter array if it was successfully created and initialized, NULL otherwise.
 */
PAdptArray CreateAdptArray(COPY_FUNC copyFunc, DEL_FUNC delFunc, PRINT_FUNC printFunc) {
    PAdptArray adptArray = (PAdptArray) malloc(sizeof(struct AdptArray_));
    if (adptArray == NULL) {
        return NULL;
    }
    
    InitAdptArray(adptArray, copyFunc, delFunc, printFunc);

    return adptArray;
}

/**
 * @brief Initialize an empty Adpt Array object
 * 
 * @param adpt_arr pointer to the adapter array
 * @param copyFunc copy function
 * @param delFunc delete function
 * @param printFunc print function
 */
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

/**
 * @brief Get the Adpt Array Size object
 * 
 * @param adpt_arr pointer to the adapter array
 * @return amount of elements in the array, -1 if there are no elements.
 */
int GetAdptArraySize(PAdptArray adpt_arr) {
    if (adpt_arr == NULL)
        return -1;

    return adpt_arr->size;
}

/**
 * @brief Get the Adpt Array At object
 * 
 * @param adpt_arr pointer to the adapter array
 * @param index the index to get
 * @return PElement copy of the element at the specified index, NULL if there's no element.
 */
PElement GetAdptArrayAt(PAdptArray adpt_arr, int index) {
    if (adpt_arr == NULL || index < 0) {
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

/**
 * @brief Set the Adpt Array At object
 * 
 * @param adpt_arr pointer to the adapter array
 * @param index the index to set
 * @param element the element to insert
 * @return Result 
 */
Result SetAdptArrayAt(PAdptArray adpt_arr, int index, PElement element) {
    if (adpt_arr == NULL || index < 0)  return FAIL;
    if (index < adpt_arr->size) {
        if(adpt_arr->d_array[index])
            adpt_arr->delFunc(adpt_arr->d_array[index]);

        if (element)
            adpt_arr->d_array[index] = adpt_arr->copyFunc(element);
        return SUCCESS;
    }

    Result result = ResizeArray(adpt_arr, index + 1);
    if (result == FAIL) {
        return FAIL;
    }
    if (element) {
        adpt_arr->d_array[index] = adpt_arr->copyFunc(element);
    }

    return SUCCESS;
}

/**
 * @brief resize the dynamic array
 * 
 * @param adpt_arr delete the elements in the adapter array.
 * @param new_size the new size the dynamic array
 * @return Result 
 */
Result ResizeArray(PAdptArray adpt_arr, int new_size) {
    PElement *new_dynamic_array = calloc(new_size, sizeof(PElement));
    if (new_dynamic_array == NULL) {
        printf("returning fail\n");
        return FAIL;
    }
    PElement *temp =  adpt_arr->d_array;
    memcpy(new_dynamic_array, adpt_arr->d_array, adpt_arr->size * sizeof(PElement));

    adpt_arr->d_array = new_dynamic_array;
    adpt_arr->size = new_size;
    free(temp);

    return SUCCESS;
}

/**
 * @brief delete the adapter array.
 * 
 * @param adpt_arr pointer to the adapter array
 */
void DeleteAdptArray(PAdptArray adpt_arr) {
    if (adpt_arr == NULL) {
        return;
    }

    DeleteDArray(adpt_arr);

    free(adpt_arr);
    adpt_arr = NULL;
} 

/**
 * @brief delete the elements in the dynamic array.
 * 
 * @param adpt_arr pointer to the adapter array
 */
void DeleteDArray(PAdptArray adpt_arr) {
    if (adpt_arr == NULL || adpt_arr->d_array == NULL) {
        return;
    }

    for (size_t i = 0; i < adpt_arr->size; i++) {
        if (adpt_arr->d_array[i]) {
            adpt_arr->delFunc(adpt_arr->d_array[i]);
        // adpt_arr->d_array[i] = NULL; // TOOD: do i need this?
        }
    }
    adpt_arr->size = 0;
    free(adpt_arr->d_array);
    adpt_arr->d_array = NULL;
} 

/**
 * @brief print the elements in the dynamic array
 * 
 * @param adpt_arr pointer to the adapter array
 */
void PrintDB(PAdptArray adpt_arr) {
    if(adpt_arr == NULL) return;

    for (size_t i = 0; i < adpt_arr->size; i++) {
        if (adpt_arr->d_array[i])
            adpt_arr->printFunc(adpt_arr->d_array[i]);
    }
}
