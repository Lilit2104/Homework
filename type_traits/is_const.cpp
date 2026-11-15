#include <iostream>

template <typename T>
struct is_const 
{
    static constexpr bool value = false;
};

template <typename T>
struct is_const<const T> 
{
    static constexpr bool value = true;
};

int main() 
{
    std::cout << std::boolalpha;
    std::cout << is_const<const int>::value << "\n";     
    std::cout << is_const<int>::value << "\n";           
    std::cout << is_const<const int*>::value << "\n";    
    std::cout << is_const<int* const>::value << "\n";    

    return 0;
}