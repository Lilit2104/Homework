#include <iostream>

template <typename T>
struct is_reference 
{
    static constexpr bool value = false;
};

template <typename T>
struct is_reference<T&> 
{
    static constexpr bool value = true;
};

template <typename T>
struct is_reference<T&&> 
{
    static constexpr bool value = true;
};

int main() 
{
    std::cout << std::boolalpha;
    std::cout << is_reference<int>::value << "\n";    
    std::cout << is_reference<int&>::value << "\n";   
    std::cout << is_reference<int&&>::value << "\n";  
    
    return 0;
}