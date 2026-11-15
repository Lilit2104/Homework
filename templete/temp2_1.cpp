#include <iostream>
#include <string>
#include <cctype>
#include <iomanip>


template <typename T>
void printValue(T value) 
{
    std::cout << "Unknown type" << std::endl;
}


template <>
void printValue<int>(int value) 
{
    std::cout << "Integer: " << value << std::endl;
}


template <>
void printValue<double>(double value) 
{
    std::cout << "Double: " << std::fixed << std::setprecision(2) << value << std::endl;
}


template <>
void printValue<char*>(char* value) 
{
    for (char* p = value; *p != '\0'; ++p)
    {
        std::cout << static_cast<char>(std::toupper(static_cast<unsigned char>(*p)));
    }
    std::cout << std::endl;
}

int main() 
{
    printValue(42);                  
    printValue(3.14159);               

    char hello[] = "hello";          
    printValue(hello);                 

    printValue(std::string("Hello"));  
    printValue(3.14f);

    return 0;
}