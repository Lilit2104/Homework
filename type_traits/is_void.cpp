#include <iostream>

template <typename T>
struct is_void 
{
    static constexpr bool value = false;
};

template <>
struct is_void<void> 
{
    static constexpr bool value = true;
};

template <>
struct is_void<const void> 
{
    static constexpr bool value = true;
};

template <>
struct is_void<volatile void> 
{
    static constexpr bool value = true;
};

template <>
struct is_void<const volatile void> 
{
    static constexpr bool value = true;
};

int main() 
{
    std::cout << std::boolalpha;
    std::cout << is_void<void>::value << "\n";        
    std::cout << is_void<int>::value << "\n";         
    std::cout << is_void<const void>::value << "\n";  
    
    return 0;
}