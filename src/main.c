#include <stdio.h>
#include <stdlib.h>
#include "string_utils.h"

// SRTCPY
/*
int main()        
{
    int n;
    printf("Please enter the size: ");
    scanf("%d", &n);

    char *src = (char*)malloc((n + 1) * sizeof(char));
    char *dest = (char*)malloc((n + 1) * sizeof(char));

    printf("Enter string: ");
    scanf("%s", src);

    char *copy = my_strcpy(dest, src);
    printf("Copied string: %s\n", copy);

    free(src);
    free(dest);

    return 0;

}
    */

// STRNCPY
/*
int main()        
{
    int size;
    printf("Enter the size: ");
    scanf("%d", &size);

    char *src= (char*)malloc((size + 1) * sizeof(char));
    char *dest = (char*)malloc((size + 1) * sizeof(char));

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
*/


// STRCAT
/*
int main()        
{
    int size;
    printf("Enter the size: ");
    scanf("%d", &size);

    char *src= (char*)malloc((size + 1) * sizeof(char));
    char *dest = (char*)malloc((size + 1) * sizeof(char));

    printf("Enter string: ");
    scanf("%s", dest);

    
    printf("Please enter the string : ");
    scanf("%s", src);

    char *copy = my_strcat(dest,src);
    printf("Copied string:   %s\n  ", copy);

    free(src);
    free(dest);

    return 0;

}
*/


//STRCMP

/*
int main()        
{
    int size;
    printf("Please enter the size: ");
    scanf("%d", &size);

    char *string1= (char*)malloc((size + 1) * sizeof(char));
    char *string2 = (char*)malloc((size + 1) * sizeof(char));

    printf("Please enter the first string: ");
    scanf("%s", string1);

    
    printf("Please enter the  2nd string : ");
    scanf("%s", string2);

    int diff = my_strcmp(string1, string2);
    printf("%d\n", diff);

    free(string1);
    free(string2);

    return 0;

}
*/


//STRNCMP
/*
    int main()        
{
    int size;
    printf("Please enter the size: ");
    scanf("%d", &size);

    char *string1= (char*)malloc((size + 1) * sizeof(char));
    char *string2 = (char*)malloc((size + 1) * sizeof(char));

    printf("Please enter the first string: ");
    scanf("%s", string1);

    
    printf("Please enter the  2nd string : ");
    scanf("%s", string2);

    int n;
    printf("Please enter the size you want to compare: ");
    scanf("%d", &n);

    int diff = my_strncmp(string1, string2,n);
    printf("%d\n", diff);

    free(string1);
    free(string2);

    return 0;

}
*/    


//STRCHR
/*
int main()        
{
    int size;
    printf("Please enter the size: ");
    scanf("%d", &size);

    char *string1= (char*)malloc((size + 1) * sizeof(char));
   

    printf("Please enter the first string: ");
    scanf("%s", string1);

    char n;
    printf("Please enter the simbol: ");
    scanf(" %c", &n);

    
    char *position = my_strchr(string1, n);

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
*/


//STRRCHR
/*
int main()        
{
    int size;
    printf("Please enter the size: ");
    scanf("%d", &size);

    char *string1= (char*)malloc((size + 1) * sizeof(char));
   

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
*/

// STRSTR
/*
int main()        
{
    int size;
    printf("Please enter the size: ");
    scanf("%d", &size);

    char *string1 = (char*)malloc((size + 1) * sizeof(char));
    char *string2 = (char*)malloc((size + 1) * sizeof(char));

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
*/



//STRSPN
/*
 int main()        
{
    int size;
    printf("Please enter the size: ");
    scanf("%d", &size);

    char *string1= (char*)malloc((size + 1) * sizeof(char));
    char *string2 = (char*)malloc((size + 1) * sizeof(char));

    printf("Please enter the first string: ");
    scanf("%s", string1);

    
    printf("Please enter the  2nd string : ");
    scanf("%s", string2);

    int count_of_sym = my_strspn (string1, string2);
    printf("COUNT IS EQUAL TO: %d\n", count_of_sym);

    free(string1);
    free(string2);

    return 0;

}
*/

//STRCSPN
/*
int main()        
{
    int size;
    printf("Please enter the size: ");
    scanf("%d", &size);

    char *string1= (char*)malloc((size + 1) * sizeof(char));
    char *string2 = (char*)malloc((size + 1) * sizeof(char));

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
*/

//STRBRK
/*
int main()        
{
    int size;
    printf("Please enter the size: ");
    scanf("%d", &size);

    char *string1= (char*)malloc((size + 1) * sizeof(char));
    char *string2 = (char*)malloc((size + 1) * sizeof(char));

    printf("Please enter the first string: ");
    scanf("%s", string1);

    
    printf("Please enter the  2nd string : ");
    scanf("%s", string2);

    char *position = my_strpbrk(string1, string2);

    if (position != NULL)
    {
        int index = position - string1;
        printf("Result: %s\n", position);
        printf("Index: %ld\n", position - string1);
    }
    else
    {
        printf("-1\n");
    }

    free(string1);
    free(string2);

    return 0;

}
*/


//STRTOK
/*
int main()
{
    int size;
    printf("Please enter the size: ");
    scanf("%d", &size);

    char *string1 = (char*)malloc((size + 1) * sizeof(char));
    char *string2 = (char*)malloc((size + 1) * sizeof(char));

    printf("Please enter the first string: ");
    scanf("%s", string1);

    printf("Please enter the 2nd string: ");
    scanf("%s", string2);

    char *token = my_strtok(string1, string2);

    while (token != NULL)
    {
        printf("Result: %s\n", token);
        token = my_strtok(NULL, string2);
    }

    free(string1);
    free(string2);

    return 0;
}
*/


//COUNT_CHAR
/*
int main()
{
    int size;
    printf("Please enter the size: ");
    scanf("%d", &size);

    char *string1 = (char*)malloc((size + 1) * sizeof(char));

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

*/

// STREQUAL
/*
int main()
{
    int size;
    printf("Please enter the size: ");
    scanf("%d", &size);

    char *string1 = (char*)malloc((size + 1) * sizeof(char));
    char *string2 = (char*)malloc((size + 1) * sizeof(char));

    printf("Please enter the first string: ");
    scanf("%s", string1);

    printf("Please enter the 2nd string: ");
    scanf("%s", string2);

    int res = my_strequal(string1, string2);

   
        printf("Result: %d\n", res);
        
    free(string1);
    free(string2);

    return 0;
}
*/


//FILL_STRING
/*
int main()
{
    int size;
    printf("Please enter the size: ");
    scanf("%d", &size);

    char *string1 = (char*)malloc((size + 1) * sizeof(char));
    char *string2 = (char*)malloc((size + 1) * sizeof(char));

    printf("Please enter the first string: ");
    scanf("%s", string1);

    printf("Please enter the 2nd string: ");
    scanf("%s", string2);

    
    my_fill_string(string1, string2[0]);

    printf("Result: %s\n", string1);

    free(string1);
    free(string2);

    return 0;
}
*/

//STRLASTINDEX
/*
int main()        
{
    int size;
    printf("Please enter the size: ");
    scanf("%d", &size);

    char *string1= (char*)malloc((size + 1) * sizeof(char));
   

    printf("Please enter the first string: ");
    scanf("%s", string1);

    char n;
    printf("Please enter the simbol: ");
    scanf(" %c", &n);

    
    int index = my_strlastindex(string1, n);
    if (index != -1)
    {
        printf("%d\n", index);
    }
    else
    {
        printf("-1\n");
    }

    free(string1);

    return 0;
}
*/

//STRREVERCE
/*
int main()        
{
    int size;
    printf("Please enter the size: ");
    scanf("%d", &size);

    char *string1= (char*)malloc((size + 1) * sizeof(char));
   
    printf("Please enter the first string: ");
    scanf("%s", string1);

    my_strreverse(string1);
    printf("Reverce string: %s\n", string1);

    free(string1);

    return 0;
}
*/

//STRPOLINDROME

int main()        
{
    int size;
    printf("Please enter the size: ");
    scanf("%d", &size);

    char *string1= (char*)malloc((size + 1) * sizeof(char));
   
    printf("Please enter the first string: ");
    scanf("%s", string1);

    int res = my_strpalindrome(string1);
    printf("Result: %d\n", res);

    free(string1);

    return 0;
}
