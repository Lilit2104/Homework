#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int main()
{
    int n, m;

    printf("Please enter the row: ");
    scanf("%d", &n);

    printf("Please enter the column: ");
    scanf("%d", &m);

    int **matrix = matrixminput(n, m);
    printf("\n");

    int **result = malloc(m * sizeof(int *));
    for(int j = 0; j < m; j++)
    {
        result[j] = malloc(m * sizeof(int));
    }

    matrixoutput(n, m, matrix);

  //  1,2
 /* int maxsum;
    int row = maxrowsum(n, m, matrix, &maxsum);

    int minsum;
    int column = mincoulsum(n, m, matrix, &minsum);

    printf("Maximum sum = %d\n", maxsum);
    printf("Row = %d\n", row + 1);

    printf("Elements: ");
    for (int j = 0; j < m; j++)
    {
        printf("%d ", matrix[row][j]);
    }
    printf("\n");


    printf("Minimum sum = %d\n", minsum);
    printf("Column = %d\n", column + 1);

    printf("Elements: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", matrix[i][column]);
    }
    printf("\n");
*/

// 3
/*
    int bound = boundaddmatr(n, m, matrix);
    printf("The sum of bound equal to %d\n", bound);
*/

//4    zigzagmat(n , m, matrix);
//5
/*
    rotatematrix(n,m, matrix,result);

    printf("Rotated matrix:\n");

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
        {

            printf("%d ", result[i][j]);
        }
        printf("\n");
    }
*/
    freematrix(matrix, n);

    return 0;
}