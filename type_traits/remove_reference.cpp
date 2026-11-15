#include <iostream>

template <typename T, typename U>
struct is_same 
{ 
    static constexpr bool value = false; 
};

template <typename T>
struct is_same<T, T> 
{ 
    static constexpr bool value = true; 
};

template <typename T>
struct remove_reference 
{
    using type = T;
};

template <typename T>
struct remove_reference<T&> 
{
    using type = T;
};

template <typename T>
struct remove_reference<T&&> 
{
    using type = T;
};

int main() 
{
    std::cout << std::boolalpha;
    std::cout << is_same<remove_reference<int>::type, int>::value << "\n";             
    std::cout << is_same<remove_reference<int&>::type, int>::value << "\n";            
    std::cout << is_same<remove_reference<int&&>::type, int>::value << "\n";            
    std::cout << is_same<remove_reference<const int&>::type, const int>::value << "\n"; 

    return 0;
}