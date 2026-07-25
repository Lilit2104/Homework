#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"


int **matrixminput(int n, int m)
{
    int **matrix = malloc(m * sizeof(int *));
    

    for(int j = 0; j < m; j++)
    {
        matrix[j] = malloc(m * sizeof(int));
    }

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            scanf("%d", &matrix[i][j]);
        }
    }
    return matrix;
}


void matrixoutput(int n, int m, int **matrix)
{
    
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            printf(" %d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int maxrowsum(int n, int m, int **matrix, int *maxsum)
{
    int index = 0;
    *maxsum = 0;

    for (int i = 0; i < n; i++)
    {
        int sum = 0;

        for (int j = 0; j < m; j++)
        {
            sum += matrix[i][j];
        }

        if (i == 0 || sum > *maxsum)
        {
            *maxsum = sum;
            index = i;
        }
    }

    return index;
}


int mincoulsum(int n, int m, int **matrix, int *minsum)
{
    int indexc = 0;
    *minsum = 0;

    for (int j = 0; j < m; j++)
    {
        int sum = 0;

        for (int i = 0; i < n; i++)
        {
            sum += matrix[i][j];
        }

        if (j == 0 || sum < *minsum)
        {
            *minsum = sum;
            indexc = j;
        }
    }
   

    return indexc;   
}


int boundaddmatr(int n, int m, int **matrix)
{
    int sum = 0;
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
          if(j==0 || i== 0 || j==m - 1 || i==n - 1)
          {
            sum += matrix[i][j];
          }
        }
        
    }
    return sum;
}


void zigzagmat(int n, int m, int **matrix)
{
    for(int i= 0; i < n; i++)
    {
        if(i % 2 == 0)
        {
            for(int j = 0; j < m; j++)
            {
                printf(" %d ", matrix[i][j]);
            }
        }
        else
        {
            for(int j = m - 1; j >= 0; j--)
            {
                printf(" %d ", matrix[i][j]);

            }
        }
        printf("\n");
    }
    
}
void rotatematrix(int n, int m, int **matrix, int **result)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            result[j][n - i - 1] = matrix[i][j];
        }
    }
}


void freematrix(int **matrix, int n)
{
    for(int i = 0; i < n; i++)
    {
        free(matrix[i]);
    }

    free(matrix);
}

