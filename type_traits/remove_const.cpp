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
struct remove_const 
{
    using type = T;
};

template <typename T>
struct remove_const<const T> 
{
    using type = T;
};

int main() 
{ 
    std::cout << is_same<remove_const<const int>::type, int>::value << "\n";        
    std::cout << is_same<remove_const<int>::type, int>::value << "\n";              
    std::cout << is_same<remove_const<const int*>::type, const int*>::value << "\n";
    std::cout << is_same<remove_const<int* const>::type, int*>::value << "\n";      
    std::cout << is_same<remove_const<const volatile int>::type, volatile int>::value << "\n"; 

    return 0;
}