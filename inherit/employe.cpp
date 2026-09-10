#include <iostream>
#include <string>

class Employee{
    protected:
        std::string name;
        int salary;

    public:

        Employee(std::string name, int salary) 
            : name(name), salary(salary) {}

        void work()
        {
            std::cout << " I am working.." << std::endl;           
        }

         int get_salary()
        {
            return salary;
        }
};

class Programmer : public Employee{
    public:
        int language_count;
    
    public:

        Programmer(std::string name, int salary, int language_count) 
            : Employee(name, salary), language_count(language_count) {}

        void writecode(int count)
        {
            std::cout << "Programmer write :  " << count << " line"<< std::endl; 
        }

        void addLanguage(std::string lang)
        {
            language_count += 1;
            std::cout << "I learn this language: " << lang << std::endl;
        }

        void addLanguage(std::string lang1, std::string lang2)
        {
            language_count += 2;
            std::cout << "I learn this languages: " << lang1 << "  ,  " << lang2 << std::endl;
        }

};



int main()
{
    Employee emp("Anna", 300000);
    emp.work();

    Programmer prog("David", 500000, 2);
    prog.work();              
    prog.writecode(150);      
    prog.addLanguage("C++");  
    prog.addLanguage("Python", "Java"); 

    std::cout << "Employee salary: " << emp.get_salary() << std::endl;
    std::cout << "Programmer salary: " << prog.get_salary() << std::endl;
    std::cout << "Programmer knows " << prog.language_count << " languages" << std::endl;

    return 0;
}