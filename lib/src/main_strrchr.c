#include <stdio.h>
#include <stdlib.h>
#include "string_utils.h"


int main()        
{
    int size;
    printf("Please enter the size: ");
    scanf("%d", &size);

    char *string1= (char*)malloc((size + 1) * sizeof(char));
   

      if(string1 == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Please enter the first string: ");
    scanf("%s", string1);

    char n;
    printf("Please enter the simbol: ");
    scanf(" %c", &n);

    
    char *position = my_strrchr(string1, n);

    if (position != NULL)
    {
        int index = position - string1;
        printf("%d\n", index + 1);
    }
    else
    {
        printf("-1\n");
    }

    free(string1);

    return 0;
}