#include <iostream>

template <unsigned int N>
struct fibonacci 
{
    static constexpr unsigned int value = fibonacci<N - 1>::value + fibonacci<N - 2>::value;
};


template <>
struct fibonacci<0> 
{
    static constexpr unsigned int value = 0;
};


template <>
struct fibonacci<1> 
{
    static constexpr unsigned int value = 1;
};

int main() 
{
    std::cout << fibonacci<0>::value << "\n";   
    std::cout << fibonacci<1>::value << "\n";   
    std::cout << fibonacci<5>::value << "\n";  
    std::cout << fibonacci<7>::value << "\n";   
    std::cout << fibonacci<10>::value << "\n"; 

    return 0;
}