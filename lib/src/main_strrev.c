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

    my_strreverse(string1);
    printf("Reverce string: %s\n", string1);

    free(string1);

    return 0;
}