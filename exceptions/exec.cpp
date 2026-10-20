#include <iostream>
#include <string>
#include <stdexcept>

//1
int divide(int a, int b)
{
    if (b == 0)
        throw std::runtime_error("Division by zero");
    return a / b;
}

void task1()
{
    std::cout << "1. Division by zero\n";
    try
    {
        std::cout << "10 / 2 = " << divide(10, 2) << "\n";
        std::cout << "10 / 0 = " << divide(10, 0) << "\n";  
        std::cout << "Այս տողը չի կատարվի\n";
    }
    catch (const std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }
}


//2

int getElement(const int* arr, size_t size, int index)
{
    if (index < 0 || static_cast<size_t>(index) >= size)
        throw std::out_of_range("Index " + std::to_string(index) +
                                " is out of range [0, " +
                                std::to_string(size - 1) + "]");
    return arr[index];
}

void task2()
{
    std::cout << "\n2. Invalid index \n";
    int arr[5] = {10, 20, 30, 40, 50};

    try
    {
        std::cout << "arr[2] = " << getElement(arr, 5, 2) << "\n";
        std::cout << "arr[7] = " << getElement(arr, 5, 7) << "\n";   
    }
    catch (const std::out_of_range& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }
}

//3

class InsufficientFundsException : public std::exception{
    public:
        const char* what() const noexcept override
        {
            return "Insufficient funds";
        }
};

class BankAccount{
    private:
        double balance;

    public:
        explicit BankAccount(double initial) : balance(initial) {}

        void withdraw(double amount)
        {
            if (amount > balance)
            {
                throw InsufficientFundsException();
            }
            balance -= amount;
        }

        double getBalance() const { return balance; }
};

void task3()
{
    std::cout << "\n3. Bank account\n";
    BankAccount acc(500);
    std::cout << "Balance: " << acc.getBalance() << "\n";

    try
    {
        std::cout << "Withdraw: 200\n";
        acc.withdraw(200);
        std::cout << "New balance: " << acc.getBalance() << "\n";

        std::cout << "Withdraw: 700\n";
        acc.withdraw(700);                                 
    }
    catch (const InsufficientFundsException& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }
}

//4

class InvalidAgeException : public std::exception{
    public:
        const char* what() const noexcept override
        {
            return "Invalid age";
        }
};

void checkAge(int age)
{
    if (age < 0 || age > 150)
        throw InvalidAgeException();
    std::cout << "Age " << age << " is valid\n";
}

void task4()
{
    std::cout << "\n4. Age check\n";
    int inputs[] = {25, -5, 200};

    for (int age : inputs)
    {
        try
        {
            std::cout << "Input: " << age << "\n";
            checkAge(age);
        }
        catch (const InvalidAgeException& e)
        {
            std::cout << "Exception: " << e.what() << "\n";
        }
    }
}

//5

class ApplicationException : public std::exception{
    public:
        const char* what() const noexcept override
        {
            return "Application error";
        }
};

class InputException : public ApplicationException{
    public:
        const char* what() const noexcept override
        {
            return "Input error: invalid user input";
        }
};

class DatabaseException : public ApplicationException{
    public:
        const char* what() const noexcept override
        {
            return "Database error: connection failed";
        }
};

class FileException : public ApplicationException{
    public:
        const char* what() const noexcept override
        {
            return "File error: file not found";
        }
};


void runScenario(int scenario)
{
    switch (scenario)
    {
        case 1: throw InputException();
        case 2: throw DatabaseException();
        case 3: throw FileException();
        case 4: throw ApplicationException();
        default: std::cout << "Scenario " << scenario << ": no error\n";
    }
}

void task5()
{
    std::cout << "\n 5. Exception hierarchy\n";

    for (int s = 0; s <= 4; ++s)
    {
        try
        {
            runScenario(s);
        }
       
        catch (const InputException& e)
        {
            std::cout << "[InputException] " << e.what() << "\n";
        }
        catch (const DatabaseException& e)
        {
            std::cout << "[DatabaseException] " << e.what() << "\n";
        }
        catch (const FileException& e)
        {
            std::cout << "[FileException] " << e.what() << "\n";
        }
        catch (const ApplicationException& e)
        {
            std::cout << "[ApplicationException] " << e.what() << "\n";
        }
    }
}

//6

void riskyFunction(int mode)
{
    switch (mode)
    {
        case 1: throw std::invalid_argument("Invalid argument was passed");
        case 2: throw std::out_of_range("Value is out of range");
        case 3: throw std::runtime_error("Runtime failure");
        default: std::cout << "Mode " << mode << ": OK\n";
    }
}

void task6()
{
    std::cout << "\n6. Multiple catch \n";

    for (int mode = 0; mode <= 3; ++mode)
    {
        try
        {
            riskyFunction(mode);
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << "[invalid_argument] " << e.what() << "\n";
        }
        catch (const std::out_of_range& e)
        {
            std::cout << "[out_of_range] " << e.what() << "\n";
        }
        catch (const std::runtime_error& e)
        {
            std::cout << "[runtime_error] " << e.what() << "\n";
        }
    }
}

int main()
{
    task1();
    task2();
    task3();
    task4();
    task5();
    task6();
    return 0;
}