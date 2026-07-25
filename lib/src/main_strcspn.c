#include <stdio.h>
#include <stdlib.h>
#include "string_utils.h"


int main()        
{
    int size;
    printf("Please enter the size: ");
    scanf("%d", &size);

    char *string1= (char*)malloc((size + 1) * sizeof(char));
    char *string2 = (char*)malloc((size + 1) * sizeof(char));


      if(string1 == NULL  && string2 == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    
    printf("Please enter the first string: ");
    scanf("%s", string1);

    
    printf("Please enter the  2nd string : ");
    scanf("%s", string2);

    int count_of_sym = my_strcspn (string1, string2);
    printf("COUNT IS EQUAL TO: %d\n", count_of_sym);

    free(string1);
    free(string2);

    return 0;

}