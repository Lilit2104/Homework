#include <stdio.h>
#include <stdlib.h>
#include "string_utils.h"


int main()        
{
    int n;
    printf("Please enter the size: ");
    scanf("%d", &n);

    char *src = (char*)malloc((n + 1) * sizeof(char));
    char *dest = (char*)malloc((n + 1) * sizeof(char));


     if(src == NULL  && dest == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }


    printf("Enter string: ");
    scanf("%s", src);

    char *copy = my_strcpy(dest, src);
    printf("Copied string: %s\n", copy);

    free(src);
    free(dest);

    return 0;

}