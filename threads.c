#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
typedef struct
{

    int *column, *row, size, size_row_M2, *OUTPUT, ResultPos;
} STRUCT_COMPUTE_ELEMENT;

typedef struct
{

    int *column, *row, NO_COLUMNS_M2, NO_ROWS_M2, *OUTPUT, ResultPos;
} STRUCT_COMPUTE_ROW;

void *COMPUTE_ELEMENT(STRUCT_COMPUTE_ELEMENT *ptr);
void *COMPUTE_ROW(STRUCT_COMPUTE_ROW *ptr);

void main(void)
{
    FILE *fp;
    char c[1000];
    char *argument;
    char *TOKENS[1000];
    int SIZE_ROW_MATRIX1, SIZE_COLUMN_MATRIX1, SIZE_ROW_MATRIX2, SIZE_COLUMN_MATRIX2;
    int TOGET_ENTRY;
    //opening file for matrix multiplication
    fp = fopen("matrix-readme.txt", "r");
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
    //assign size of elements from extracted data
    SIZE_ROW_MATRIX1 = atoi(TOKENS[0]);
    SIZE_COLUMN_MATRIX1 = atoi(TOKENS[1]);
    TOGET_ENTRY = SIZE_ROW_MATRIX1 * SIZE_COLUMN_MATRIX1;
    SIZE_ROW_MATRIX2 = atoi(TOKENS[TOGET_ENTRY + 2]);
    SIZE_COLUMN_MATRIX2 = atoi(TOKENS[TOGET_ENTRY + 3]);
    //if not matrix multiplication is not possible
      if(SIZE_COLUMN_MATRIX1!=SIZE_ROW_MATRIX2)
      {
          printf("NOT POSSIBLE MULTIPLICATION,EDIT YOUR ENTRY")
         return 0; 
      }
    int MATRIX1[SIZE_ROW_MATRIX1][SIZE_COLUMN_MATRIX1];
    int MATRIX2[SIZE_ROW_MATRIX2][SIZE_COLUMN_MATRIX2];
    int start = 2;
     //assign values of first Matrix from extracted data
    for (int i = 0; i < SIZE_ROW_MATRIX1; i++)
    {
        for (int j = 0; j < SIZE_COLUMN_MATRIX1; j++)
        {
            MATRIX1[i][j] = atoi(TOKENS[start]);
            start++;
        }
    }
    //get entry of values of the second matrix in the file
    start = TOGET_ENTRY + 4;
    //assign values of second Matrix fro extracted data
    for (int i = 0; i < SIZE_ROW_MATRIX2; i++)
    {
        for (int j = 0; j < SIZE_COLUMN_MATRIX2; j++)
        {
            MATRIX2[i][j] = atoi(TOKENS[start]);
            start++;
        }
    }

    int Iterator_For_Thread_Creation = 0;
    int NO_NEEDED_THREADS_ELEMENT = SIZE_ROW_MATRIX1 * SIZE_COLUMN_MATRIX2;
    //create array of threads
    pthread_t thread[NO_NEEDED_THREADS_ELEMENT];
    int ret_thread[NO_NEEDED_THREADS_ELEMENT];
    //create array of structs that are sent as arguments to the functions performed by threads
    STRUCT_COMPUTE_ELEMENT *ptrstruct1[NO_NEEDED_THREADS_ELEMENT];
    STRUCT_COMPUTE_ELEMENT struct1[NO_NEEDED_THREADS_ELEMENT];
     //assign pointers for each struct
    for (int i = 0; i < NO_NEEDED_THREADS_ELEMENT; i++)
    {
        ptrstruct1[i] = &struct1[i];
    }
    int RESULT_COMPUTE_ELEMENT[SIZE_ROW_MATRIX1][SIZE_COLUMN_MATRIX2];
    int row = 0;

    clock_t timeBegin,timeEnd;
    //set clock begin
    timeBegin=clock();
    //loop to call threads
     while (Iterator_For_Thread_Creation < NO_NEEDED_THREADS_ELEMENT)
    {
        for (int x = 0; x < SIZE_COLUMN_MATRIX2; x++)
        {
            //assign values of struct that will be sent to the function
            struct1[Iterator_For_Thread_Creation].row = &MATRIX1[row];
            struct1[Iterator_For_Thread_Creation].column = &(MATRIX2[0][x]);
            struct1[Iterator_For_Thread_Creation].size = SIZE_COLUMN_MATRIX1;
            struct1[Iterator_For_Thread_Creation].size_row_M2 = SIZE_COLUMN_MATRIX2;
            struct1[Iterator_For_Thread_Creation].ResultPos = Iterator_For_Thread_Creation;
            struct1[Iterator_For_Thread_Creation].OUTPUT = &RESULT_COMPUTE_ELEMENT;

            ret_thread[Iterator_For_Thread_Creation] = pthread_create(&thread[Iterator_For_Thread_Creation], NULL, COMPUTE_ELEMENT, (STRUCT_COMPUTE_ELEMENT *)ptrstruct1[Iterator_For_Thread_Creation]);

            if (ret_thread[Iterator_For_Thread_Creation])

            {
                fprintf(stderr, "Error - pthread_create() return code: %d\n", ret_thread[Iterator_For_Thread_Creation]);
                exit(EXIT_FAILURE);
            }
            Iterator_For_Thread_Creation++;
        }
        row++;
    }

    //JOINING
    for (int i = 0; i < NO_NEEDED_THREADS_ELEMENT; i++)
    {
        pthread_join(thread[i], NULL);
    }
    //end of clock
    timeEnd=clock();
    //print OUTPUT matrix from  COMPUTE ELEMENT
    FILE *fptr;
    fptr = fopen("RESULT_MATRIX.txt", "w");

    fprintf(fptr, "RESULT OF ELEMENT BY ELEMENT MULTIPLICATION\n");
    //time taken by the MATRIX Multiplication element by element
    fprintf(fptr,"TIME TAKEN = %ld \n",(timeEnd-timeBegin));
    //print the output matrix in th file
    for (int i = 0; i < SIZE_ROW_MATRIX1; i++)
    {
        for (int y = 0; y < SIZE_COLUMN_MATRIX2; y++)
        {
            fprintf(fptr, "%d\t", RESULT_COMPUTE_ELEMENT[i][y]);
        }
        fprintf(fptr, "\n");
    }


    //COMPUTE ROW BY ROW

    pthread_t thread_ROW[SIZE_ROW_MATRIX1];
    int ret_thread_ROW[SIZE_ROW_MATRIX1];

    STRUCT_COMPUTE_ROW *ptrstruct2[SIZE_ROW_MATRIX1];
    STRUCT_COMPUTE_ROW struct2[SIZE_ROW_MATRIX1];
    //assign pointers of struct to struct
    for (int i = 0; i < SIZE_ROW_MATRIX1; i++)
    {
        ptrstruct2[i] = &struct2[i];
    }
    int RESULT_COMPUTE_ROW[SIZE_ROW_MATRIX1][SIZE_COLUMN_MATRIX2];
    //set time begin
timeBegin=clock();
    for (int x = 0; x < SIZE_ROW_MATRIX1; x++)
    {   //assign values of each struct sent to function
        struct2[x].row = &MATRIX1[x];
        struct2[x].column = &(MATRIX2[0][0]);
        struct2[x].NO_ROWS_M2 = SIZE_ROW_MATRIX2;
        struct2[x].NO_COLUMNS_M2 = SIZE_COLUMN_MATRIX2;
        struct2[x].ResultPos = x * SIZE_COLUMN_MATRIX2;
        struct2[x].OUTPUT = &RESULT_COMPUTE_ROW;

        ret_thread_ROW[x] = pthread_create(&thread_ROW[x], NULL, COMPUTE_ROW, (STRUCT_COMPUTE_ROW *)ptrstruct2[x]);

        if (ret_thread_ROW[x])
        {
            fprintf(stderr, "Error - pthread_create() return code: %d\n", ret_thread_ROW[x]);
            exit(EXIT_FAILURE);
        }
    }
    //JOINING
    for (int i = 0; i < SIZE_ROW_MATRIX1; i++)
    {
        pthread_join(thread_ROW[i], NULL);
    }
    //set time end
    timeEnd=clock();

    fprintf(fptr, "RESULT OF MULTIPLICATION BY ROW \n");
    //print in file time taken
     fprintf(fptr,"TIME TAKEN = %ld\n",(timeEnd-timeBegin));
    //print in file values of second matrix
    for (int i = 0; i < SIZE_ROW_MATRIX1; i++)
    {
        for (int y = 0; y < SIZE_COLUMN_MATRIX2; y++)
        {
            fprintf(fptr, "%d\t", RESULT_COMPUTE_ROW[i][y]);
        }
        fprintf(fptr, "\n");
    }
    fclose(fptr);


}
void *COMPUTE_ELEMENT(STRUCT_COMPUTE_ELEMENT *ptr)
{
    int result = 0;
    int i = 0;
    //temp pointers
    int *ptrtemprow = (*ptr).row;
    int *ptrtempcolumn = (*ptr).column;
    int *ptrtempOutput = (*ptr).OUTPUT;
    //loop until get the value of an element
    while (i < (*ptr).size)
    {
        result += *(ptrtempcolumn)*(*ptrtemprow);
        //increment row pointer
        ptrtemprow++;
        //incremenet column pointer by the value o the number of columns in the second matrix
        ptrtempcolumn += (*ptr).size_row_M2;
        i++;
    }
//assign the result in the output matrix using its temp pointer
    ptrtempOutput += (*ptr).ResultPos;
    (*ptrtempOutput) = result;
}

void *COMPUTE_ROW(STRUCT_COMPUTE_ROW *ptr)
{

    int i = 0;
    int result = 0;
    //temp pointers
    int *ptrtemprow = (*ptr).row;
    int *ptrtempcolumn = (*ptr).column;
    int *ptrtempOutput = (*ptr).OUTPUT;
    ptrtempOutput += (*ptr).ResultPos;
    //loop until get the value of the row
    for (int x = 0; x < (*ptr).NO_COLUMNS_M2; x++)
    { 
        // loop until get hte value of the element
        while (i < (*ptr).NO_ROWS_M2)
        {
            result += *(ptrtempcolumn) * (*ptrtemprow);
            ptrtemprow++;
            ptrtempcolumn += (*ptr).NO_COLUMNS_M2;
            i++;
        }
        //assign the result to the output matrix using its temp pointer
        *ptrtempOutput = result;
        ptrtempOutput++;
        //getting back the pointer row to its position
        ptrtemprow = (*ptr).row;
        //getting back the pointer column to its next position
        ptrtempcolumn = (*ptr).column + x + 1;
        //resetting the result
        result = 0;
        i = 0;
    }
}
