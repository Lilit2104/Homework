#include <iostream>

namespace v1
{
    void printAll() 
    {
        std::cout << "No arguments provided." << std::endl;
    }

    template <typename T, typename... Args>
    void printAll(T first, Args... args)
    {
        std::cout << first << std::endl;
        printAll(args...);
    }
}

namespace v2 
{
    template <typename... Args>
    void printAll(Args... args) 
    {
        ((std::cout << args << '\n'), ...);   
    }
}

int main() 
{
    std::cout << "recursive:\n";
    v1::printAll(10, 20, 30);
    
    v1::printAll(10, 3.14, 'A', "Hello");
    v1::printAll();                 

    std::cout << "\nfold:\n";
    v2::printAll(10, 20, 30);
    
    v2::printAll(10, 3.14, 'A', "Hello");
    v2::printAll();                

    return 0;
}