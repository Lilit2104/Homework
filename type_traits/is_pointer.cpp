#include <iostream>

template <typename T>
struct is_pointer 
{
    static constexpr bool value = false;
};


template <typename T>
struct is_pointer<T*> 
{
    static constexpr bool value = true;
};


template <typename T>
struct is_pointer<T* const> 
{
    static constexpr bool value = true;
};

template <typename T>
struct is_pointer<T* volatile> 
{
    static constexpr bool value = true;
};

template <typename T>
struct is_pointer<T* const volatile> 
{
    static constexpr bool value = true;
};

int main() 
{
    std::cout << std::boolalpha;
    std::cout << is_pointer<int*>::value << "\n";        
    std::cout << is_pointer<int>::value << "\n";         
    std::cout << is_pointer<const int*>::value << "\n";  
    std::cout << is_pointer<int* const>::value << "\n";  

    return 0;
}