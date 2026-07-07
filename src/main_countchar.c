#include <stdio.h>
#include <stdlib.h>
#include "string_utils.h"



int main()
{
    int size;
    printf("Please enter the size: ");
    scanf("%d", &size);

    char *string1 = (char*)malloc((size + 1) * sizeof(char));

    if(string1 == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    char ch;

    printf("Please enter the first string: ");
    scanf("%s", string1);

    printf("Please enter the char: ");
    scanf(" %c", &ch);

    int count_of_sym = my_count_char(string1, ch);

    printf("COUNT IS EQUAL TO: %d\n", count_of_sym);

    free(string1);

    return 0;
}