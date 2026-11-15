#include <iostream>

namespace v1 
{
    template <int Base, unsigned int Exp>
    struct Power 
    {
        static constexpr int value = Base * Power<Base, Exp - 1>::value;
    };

    template <int Base>
    struct Power<Base, 0> 
    {
        static constexpr int value = 1;
    };
}

int main() 
{
    std::cout << "simple:\n";
    std::cout << v1::Power<2, 0>::value << "\n";    
    std::cout << v1::Power<2, 3>::value << "\n";    
    std::cout << v1::Power<3, 4>::value << "\n";    
    std::cout << v1::Power<5, 3>::value << "\n";    
    std::cout << v1::Power<10, 2>::value << "\n";   
    std::cout << v1::Power<2, 10>::value << "\n";   

    return 0;
}