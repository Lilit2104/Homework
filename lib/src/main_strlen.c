#include <stdio.h>
#include <stdlib.h>
#include "string_utils.h"


int main()
{
    int size;
    printf("Enter string size: ");
    scanf("%d", &size);

    char *str = (char*)malloc((size + 1) * sizeof(char));


      if(str == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }
    if(str == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Enter string: ");
    scanf("%s", str);

    int len = my_strlen(str);

    printf("Length of string: %d\n", len);

    free(str);

    return 0;
}