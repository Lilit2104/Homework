#include <stdio.h>
#include <stdlib.h>
#include "string_utils.h"


int main()        
{
    int size;
    printf("Please enter the size: ");
    scanf("%d", &size);

    char *string1 = (char*)malloc((size + 1) * sizeof(char));
    char *string2 = (char*)malloc((size + 1) * sizeof(char));


      if(string1 == NULL  && string2 == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    
    printf("Please enter the string: ");
    scanf("%s", string1);

    printf("Please enter the substring: ");
    scanf("%s", string2);

    char *position = my_strstr(string1, string2);

    if (position != NULL)
    {
        int index = position - string1;
        printf("Found at index: %d\n", index);
        printf("Result: %s\n", position);
    }
    else
    {
        printf("-1\n");
    }

    free(string1);
    free(string2);

    return 0;
}