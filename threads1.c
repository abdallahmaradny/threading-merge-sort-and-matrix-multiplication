
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
typedef struct
{

    int *array, left, mid, right;
} STRUCT_ARR_MERGE;
void merge(STRUCT_ARR_MERGE *ptr);
void *mergeSort(STRUCT_ARR_MERGE *ptr);


int main()
{
    FILE *fp;

    char c[1000];
    char *argument;
    char *TOKENS[1000];

    fp = fopen("merge-sort-readme.txt", "r");
    //reading till end of file
    fscanf(fp, "%[^EOF]", c);
    fclose(fp);
    argument = strtok(c, " \n");
    int position = 0;
    //divide read file and get output from it
    while (argument != NULL)
    {
        TOKENS[position] = argument;

        position++;
        argument = strtok(NULL, " \n");
    }
    //extract size of array
    int arr_size = atoi(TOKENS[0]);
    int arr[arr_size];
    //assign extracted values to array
    for (int i = 0; i < arr_size; i++)
    {
        arr[i] = atoi(TOKENS[i + 1]);
    }    
    //main thread
    pthread_t thread;
    int ret_thread;
     
    STRUCT_ARR_MERGE struct1;
    STRUCT_ARR_MERGE *ptrstruct;
    //assigning struct for array ,left,right and mid values
    struct1.array = &arr;
    struct1.left = 0;
    struct1.right = arr_size - 1;
    struct1.mid = arr_size / 2;
    ptrstruct = &struct1;
    ret_thread = pthread_create(&thread, NULL, mergeSort, (STRUCT_ARR_MERGE *)ptrstruct);
    if (ret_thread)
    {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", ret_thread);
        exit(EXIT_FAILURE);
    }
    pthread_join(thread, NULL);
       
    
    FILE *fptr;
    fptr = fopen("RESULT_MERGE.txt", "w");
    fprintf(fptr,"Sorted array is \n");
for(int i=0;i<arr_size;i++)
{
            fprintf(fptr, "%d  ", arr[i]);

}
   fclose(fptr);
   return 0;
}

void merge(STRUCT_ARR_MERGE *ptr)
{
    int i, j, k;
    //set values for temp arrays sizes
    int n1 = (*ptr).mid - (*ptr).left + 1;
    int n2 = (*ptr).right - (*ptr).mid;

    // create temp arrays
    int L[n1], R[n2];

    //set temp arrays
    for (i = 0; i < n1; i++)
   { 
          L[i] = *((*ptr).array + (*ptr).left + i);
   }
    for (j = 0; j < n2; j++)
        {R[j] = *((*ptr).array + (*ptr).mid + j +1);
        }
    i = 0;           
    j = 0;           
    k = (*ptr).left; 
    //compare and merge
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {

            *((*ptr).array + k) = L[i];
            i++;
        }
        else
        {
            *((*ptr).array + k) = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there 
       are any */
    while (i < n1)
    {
        *((*ptr).array + k) = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there 
       are any */
    while (j < n2)
    {
        *((*ptr).array + k) = R[j];
        j++;
        k++;
    }
}
void *mergeSort(STRUCT_ARR_MERGE *ptr)
{

    //when right greater or equal to left then dividing stops
    if ((*ptr).left < (*ptr).right)
    {
        //set min
        int m = (*ptr).left + ((*ptr).right - (*ptr).left) / 2;
        //set 2 threads one for each division
        pthread_t thread1, thread2;
        int ret_thread1, ret_thread2;

        STRUCT_ARR_MERGE struct1, struct2, struct_merge;
        STRUCT_ARR_MERGE *ptrstruct1, *ptrstruct2, *ptrstruct_merge;
        //assign values of first struct
        struct1.array = (*ptr).array;
        struct1.left = (*ptr).left;
        struct1.right = m;
         ptrstruct1=&struct1;
        //mergesort the first half
        ret_thread1 = pthread_create(&thread1, NULL, mergeSort, (STRUCT_ARR_MERGE *)ptrstruct1);
        if (ret_thread1)
        {
            fprintf(stderr, "Error - pthread_create() return code: %d\n", ret_thread1);
            exit(EXIT_FAILURE);
        }
        //assign values of second half
        struct2.array = (*ptr).array;
        struct2.left = m + 1;
        struct2.right = (*ptr).right;
        ptrstruct2=&struct2;
        //merge sort second half
        ret_thread2 = pthread_create(&thread2, NULL, mergeSort, (STRUCT_ARR_MERGE *)ptrstruct2);
        if (ret_thread2)
        {
            fprintf(stderr, "Error - pthread_create() return code: %d\n", ret_thread2);
            exit(EXIT_FAILURE);
        }
         //join the 2 threads to start merging them
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
          //start merging 
        //assigning values of merge struct array
        ptrstruct_merge = &struct_merge;
        struct_merge.array = (*ptr).array;
        struct_merge.left = (*ptr).left;
        struct_merge.right = (*ptr).right;
        struct_merge.mid = m;
        merge(ptrstruct_merge);
    }
}
