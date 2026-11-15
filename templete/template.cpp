
// 1
/*

#include <iostream>
#include <string>

class Student {
public:
    std::string name;
    int grade;

    Student(std::string name, int grade)
        : name(name), grade(grade) {}

    bool operator>(const Student& other) const 
    {
        return grade > other.grade;
    }
};

template <typename T>
T maximum(T a, T b)
{
    return (a > b) ? a : b;
}

int main()
{
    Student s1("Anna", 85);
    Student s2("Aram", 95);

    Student result = maximum(s1, s2);

    std::cout << result.name << '\n';

    return 0;
}

*/


// 2
/*
#include <iostream>

template <typename T>
void swapValues(T& a, T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

int main()
{
    int a = 10;
    int b = 20;

    swapValues(a, b);

    std::cout << a << ' ' << b << '\n';

    return 0;
}

*/


// 3
/*
#include <iostream>
#include <cstddef>

template <typename T, std::size_t N>
void printArray(const T (&arr)[N])
{
    for (std::size_t i = 0; i < N; ++i)
    {
        std::cout << arr[i] << ' ';
    }

    std::cout << '\n';
}

int main()
{
    int numbers[] = {1, 2, 3, 4, 5};
    double values[] = {1.5, 2.5, 3.5};
    char letters[] = {'A', 'B', 'C'};

    printArray(numbers);
    printArray(values);
    printArray(letters);

    return 0;
}

*/


// 4

/*

#include <iostream>
#include <cstddef>

template <typename T, std::size_t N>
std::size_t countEqual(const T (&arr)[N], const T& value)
{
    std::size_t count = 0;

    for (std::size_t i = 0; i < N; ++i) 
    {
        if (arr[i] == value) 
        {
            ++count;
        }
    }

    return count;
}

int main()
{
    int numbers[] = {1, 2, 3, 2, 2, 4, 5};

    std::cout << countEqual(numbers, 2) << '\n';

    double values[] = {1.5, 2.5, 1.5, 3.5};

    std::cout << countEqual(values, 1.5) << '\n';

    return 0;
}

*/

// 5
/*
#include <iostream>
#include <cstddef>

template <std::size_t N, typename T>
void printNTimes(const T& value)
{
    for (std::size_t i = 0; i < N; ++i)
    {
        std::cout << value << '\n';
    }
}

int main()
{
    printNTimes<3>(100);

    printNTimes<5>("Hello");

    return 0;
}

*/

// 6
/*
#include <iostream>
#include <cstddef>

template <std::size_t N, typename T>
constexpr T power(T number)
{
    T result = 1;

    for (std::size_t i = 0; i < N; ++i) 
    {
        result *= number;
    }

    return result;
}

int main()
{
    std::cout << power<3>(2) << '\n';
    std::cout << power<4>(3) << '\n';
    std::cout << power<0>(10) << '\n';

    return 0;
}

*/


// 7

/*
#include <iostream>

template <int N>
constexpr bool isEven()
{
    return N % 2 == 0;
}

int main()
{
    std::cout << isEven<10>() << '\n';
    std::cout << isEven<7>() << '\n';
    std::cout << isEven<0>() << '\n';

    static_assert(isEven<10>());
    static_assert(!isEven<7>());

    return 0;
}

*/