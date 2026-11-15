#include <iostream>
#include <string>
#include <cmath>


template <typename T, typename U>
auto multiply(T a, U b) 
{
    return a * b;
}


template <>
auto multiply<double, double>(double a, double b) 
{
    return std::round(a * b * 100.0) / 100.0;
}


template <>
auto multiply<std::string, int>(std::string a, int b) 
{
    std::string result;
    for (int i = 0; i < b; ++i)
        result += a;
    return result;
}

int main() 
{
    std::cout << multiply(5, 3) << std::endl;                 
    std::cout << multiply(2.345, 3.0) << std::endl;            
    std::cout << multiply(std::string("Hi"), 3) << std::endl;  

    return 0;
}