#ifndef MY_STRING_H
#define MY_STRING_H


int my_strlen(const char *str);    //done
char *my_strcpy(char *destination, const char *source); //done
char *my_strncpy(char *destination, const char *source, int n); // done
char *my_strcat(char *destination, const char *source); // done
int my_strcmp(const char *str1, const char *str2);  // done
int my_strncmp(const char *str1, const char *str2, int n); //done
char *my_strchr(const char *str, int ch); //done
char *my_strrchr(const char *str, int ch); //done
char *my_strstr(const char *haystack, const char *needle); //done
int my_strspn(const char *str, const char *accept); //done
int my_strcspn(const char *str, const char *reject);  // done
char *my_strpbrk(const char *str, const char *accept); //done
char *my_strtok(char *str, const char *delim);  //done
int my_count_char(const char *str, char ch); //done
int my_strequal(const char *str1, const char *str2);  //done
void my_fill_string(char *str, char ch);   //done
int my_strlastindex(const char *str, char ch);  //done
void my_strreverse(char *str);   //done
int my_strpalindrome(const char *str); //done

#endif


