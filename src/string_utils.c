#include <stdio.h>
#include <stdlib.h>
#include "string_utils.h"



int my_strlen(const char *str)
{
    if(str == NULL)
    {
        exit(1);
    }

    int count = 0;

    while(*str != '\0')
    {
        count++;
        str++;
    }
    
    return count;
}


char *my_strcpy(char *destination, const char *source)
{
    char *st = destination;

    while (*source != '\0')
    {
        *destination = *source;
        source++;
        destination++;
    }

    *destination = '\0';

    return st;
}


char *my_strncpy(char *destination, const char *source, int n)
{
   char *st = destination;
   int i = 0;

   while (i < n && source[i] != '\0')
    {
        destination[i] = source[i];
        i++;
    }

    destination[i] = '\0';

    return st;
}


char *my_strcat(char *destination, const char *source)
{
       char *st = destination;

       while(*destination != '\0')
       {
        destination++;
       }

       while (*source != '\0')
       {
         *destination = *source;
         destination++;
         source++;
       }

       *destination = '\0';

       return st;
}


int my_strcmp(const char *str1, const char *str2)
{
    
    int i = 0; 
    
    while(str1[i]!= '\0' || str2[i] != '\0')
    {
        if(str1[i] > str2[i])
        {
            return 1;            
        }

        else if(str1[i] < str2[i])
        {
            return -1;
        }

        i++;
    }

    return 0;
}



int my_strncmp(const char *str1, const char *str2, int n)
{
    int i = 0;

    while (i < n && (str1[i] != '\0' || str2[i] != '\0'))
    {
        if (str1[i] > str2[i])
            return 1;
        if (str1[i] < str2[i])
            return -1;

        i++;
    }

    return 0;
}



char *my_strchr(const char *str, int ch)
{
    int i = 0;
    char* index = NULL;

    while(str[i] != '\0')
    {
        if(str[i] == ch)
        {
            index = (char*)(str + i);
            break;
        }

        i++;
    }


      if (index == NULL && ch == '\0')
    {
        index = (char*)(str + i);
    }

    return index;
}

char *my_strrchr(const char *str, int ch)
{
    int i = 0;
    char* index = NULL;

    while(str[i] != '\0')
    {
        if(str[i] == ch)
        {
            index = (char*)(str + i);
        }

        i++;
    }

    return index;
}

char *my_strstr(const char *haystack, const char *needle)
{
    if (*needle == '\0')
    {
        return (char*)haystack;
    }

    int i = 0;
    while (haystack[i] != '\0')
    {
        int j = 0;
        while (haystack[i + j] != '\0' && needle[j] != '\0' &&
               haystack[i + j] == needle[j])
        {
            j++;
        }

        if (needle[j] == '\0')
        {
            return (char*)(haystack + i);
        }

        i++;
    }

    return NULL;
}



int my_strspn(const char *str, const char *accept)
{
    int count = 0;
    int i = 0;

    while(str[i] != '\0')
    {
        int j = 0;

        while(accept[j] != '\0')
        {
            
            if(str[i] == accept[j])
            {
                break;
            }

            j++;
        }

        if(accept[j] == '\0')
        {
            break;
        }

        count++;
        i++;
    }

      return count;
}


int my_strcspn(const char *str, const char *reject)
{
     int count = 0;
     int i = 0;

     while(str[i] != '\0')
    {
        int j = 0;

        while(reject[j] != '\0')
        {
            
            if(str[i] == reject[j])
            {
                return count;
            }

            j++;
        }

        count++;
        i++;
    }

      return count;
    
}


char *my_strpbrk(const char *str, const char *accept)
{
    int i = 0;

    while (str[i] != '\0')
    {
        int j = 0;

        while (accept[j] != '\0')
        {
            if (str[i] == accept[j])
            {
                return (char *)(str + i);
            }
            j++;
        }

        i++;
    }

    return NULL;
}


char *my_strtok(char *str, const char *delim)
{
    static char *next = NULL;
   
    if (str != NULL)
    {
        next = str;
    }
    
    if (next == NULL)
    {
        return NULL;
    }
    
    while (*next != '\0')
    {
        int found = 0;

        for (int i = 0; delim[i] != '\0'; i++)
        {
            if (*next == delim[i])
            {
                found = 1;
                break;
            }
        }

        if (!found)
            break;

        next++;
    }
    
    if (*next == '\0')
    {
        next = NULL;
        return NULL;
    }
   
    char *token = next;
   
    while (*next != '\0')
    {
        int found = 0;

        for (int i = 0; delim[i] != '\0'; i++)
        {
            if (*next == delim[i])
            {
                found = 1;
                break;
            }
        }

        if (found)
        {
            *next = '\0';
            next++;
            return token;
        }

        next++;
    }    
    next = NULL;
    return token;

}


int my_count_char(const char *str, char ch)
{
    int count = 0;

    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == ch)
        {
            count++;
        }
        i++;
    }

    return count;
}

int my_strequal(const char *str1, const char *str2)
{
    int i = 0;

    while(str1[i] != '\0' && str2[i] != '\0')
    {
        if(str1[i] != str2[i])
        {
            return 0;
        }
        i++;
    }
 
    if(str1[i] == '\0' && str2[i] == '\0')
    {
        return 1;
    }
 
    return 0;
}


void my_fill_string(char *str, char ch)
{
    int i = 0;
    while(str[i] != '\0')
    {
        str[i] = ch;
        i++;
    }
}


int my_strlastindex(const char *str, char ch)
{
     int i = 0;
     int index = -1;

    while(str[i] != '\0')
    {
        if(str[i] == ch)
        {
            index = i;
        }
        i++;
    }

    return index;
}


 

void my_strreverse(char *str)
{
    int len = my_strlen(str);
    int left = 0;
    int right = len - 1;

    while(left < right)
    {
        int temp = str[left];
        str[left] = str[right];
        str[right] = temp;

        left++;
        right--;
    }
}

int my_strpalindrome(const char *str)
{
    int len = my_strlen(str);
    int left = 0;
    int right = len - 1;

    while(left < right)
    {
        if(str[left] != str[right])
        {
            return 0;
        }
        left++;
        right--;
    }

    return 1;
}
