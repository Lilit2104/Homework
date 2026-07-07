#include <stdio.h>
#include <stdlib.h>
#include "string_utils.h"


int main()        
{
    int size;
    printf("Enter the size: ");
    scanf("%d", &size);

    char *src= (char*)malloc((size + 1) * sizeof(char));
    char *dest = (char*)malloc((size + 1) * sizeof(char));


      if(src == NULL  && dest == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }


    printf("Enter string: ");
    scanf("%s", src);

    int n;
    printf("Please enter the size you want to copy: ");
    scanf("%d", &n);

    char *copy = my_strncpy(dest,src,n);
    printf("Copied string: %s\n", copy);

    free(src);
    free(dest);

    return 0;

}